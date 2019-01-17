/**
 * @file   DefaultSink.cpp
 * @brief  DefaultSink class implementation.
 * @author zer0
 * @date   2018-09-13
 * @date   2018-10-01 (Rename: DummySink -> MemorySink)
 * @date   2018-10-02 (Rename: MemorySink -> DefaultSink)
 */

#include <libc2rtsp/client/DefaultSink.hpp>
#include <libc2rtsp/log/Log.hpp>
#include <libc2rtsp/sink/SinkFactory.hpp>

#include <H264VideoRTPSource.hh> // for parseSPropParameterSets()

#include <cassert>
#include <cstdio>
#include <sstream>

namespace libc2rtsp {
namespace client    {

TBAG_CONSTEXPR static unsigned char const NAL_START_CODE[4] = {0x00, 0x00, 0x00, 0x01};
static_assert(sizeof(NAL_START_CODE) == 4, "Why not ?");

void __after_getting_frame_cb(void * client_data, unsigned frame_size,
                              unsigned truncated_bytes,
                              struct timeval presentation_time,
                              unsigned duration_in_microseconds)
{
    auto * sink = (DefaultSink*)client_data;
    assert(sink != nullptr);
    sink->onAfterGettingFrame(frame_size, truncated_bytes, presentation_time, duration_in_microseconds);
}

void __source_closure_cb(void * client_data)
{
    auto * sink = (DefaultSink*)client_data;
    assert(sink != nullptr);
    sink->onSourceClosure();
}

// --------------------------
// DefaultSink implementation
// --------------------------

DefaultSink::DefaultSink(UsageEnvironment & env,
                         MediaSubsession & subsession,
                         char const * sink_url,
                         char const * stream_id)
        : MediaSink(env),
          _subsession(subsession),
          _receive_buffer(RECEIVE_BUFFER_SIZE),
          _stream_id(stream_id),
          _writer(libc2rtsp::sink::SinkFactory().gen(sink_url)),
          _verbose(true),
          _sprop_parameter_sets(),
          _have_written_first_frame(false)
{
    if (::strcmp(subsession.codecName(), "H264") == 0) {
        // For H.264 video stream, we use a special sink that adds 'start codes',
        // and (at the start) the SPS and PPS NAL units:
        _sprop_parameter_sets.emplace_back(std::string(subsession.fmtp_spropparametersets()));

    } else if (::strcmp(subsession.codecName(), "H265") == 0) {
        // For H.265 video stream, we use a special sink that adds 'start codes',
        // and (at the start) the VPS, SPS, and PPS NAL units:
        _sprop_parameter_sets.emplace_back(std::string(subsession.fmtp_spropvps())); // VPS
        _sprop_parameter_sets.emplace_back(std::string(subsession.fmtp_spropsps())); // SPS
        _sprop_parameter_sets.emplace_back(std::string(subsession.fmtp_sproppps())); // PPS

    } else {
        crLogE("DefaultSink::DefaultSink() Unsupported subsession: {}/{}",
               subsession.mediumName(), subsession.codecName());
        throw std::bad_alloc();
    }
}

DefaultSink::~DefaultSink()
{
    // EMPTY.
}

DefaultSink * DefaultSink::createNew(UsageEnvironment & env,
                                     MediaSubsession & subsession,
                                     char const * sink_url,
                                     char const * stream_id)
{
    return new DefaultSink(env, subsession, sink_url, stream_id);
}

std::string DefaultSink::getFrameInfo(unsigned frame_size,
                                      unsigned truncated_bytes,
                                      struct timeval const & presentation_time)
{
    std::stringstream ss;
    ss << "GetFrame(" << _stream_id << ") " << _subsession.mediumName() << '/' << _subsession.codecName()
       << " Recv(" << frame_size << ')';
    if (truncated_bytes > 0) {
        ss << " Trunc(" << truncated_bytes << ')';
    }

    char microseconds[6+1/*(NULL)*/] = {0,};
    sprintf(microseconds, "%06u", (unsigned)presentation_time.tv_usec);
    ss << " Presentation(" << presentation_time.tv_sec << '.' << microseconds << ')';

    if (_subsession.rtpSource() != nullptr && !_subsession.rtpSource()->hasBeenSynchronizedUsingRTCP()) {
        ss << " !"; // Indicate that this presentation time is not RTCP-synchronized.
    }
    ss << " NPT(" << _subsession.getNormalPlayTime(presentation_time) << ')';

    return ss.str();
}

void DefaultSink::onAfterGettingFrame(unsigned frame_size,
                                      unsigned truncated_bytes,
                                      struct timeval const & presentation_time,
                                      unsigned UNUSED_PARAM(duration_in_microseconds))
{
    crLogIfD(_verbose, getFrameInfo(frame_size, truncated_bytes, presentation_time));

    if (!_have_written_first_frame) {
        // If we have NAL units encoded in "sprop parameter strings",
        // prepend these to the file:

        for (auto & param : _sprop_parameter_sets) {
            unsigned int sprop_records_size = 0;

            // Returns the binary value of each 'parameter set' specified in a "sprop-parameter-sets" string
            // (in the SDP description for a H.264/RTP stream).
            //
            // The value is returned as an array (length "numSPropRecords") of "SPropRecord"s.
            // This array is dynamically allocated by this routine, and must be delete[]d by the caller.
            SPropRecord * sprop_records = parseSPropParameterSets(param.data(), sprop_records_size);
            for (unsigned int i = 0; i < sprop_records_size; ++i) {
                write(NAL_START_CODE, sizeof(NAL_START_CODE), presentation_time);
                write(sprop_records[i].sPropBytes, sprop_records[i].sPropLength, presentation_time);
            }
            delete [] sprop_records;
        }
        _have_written_first_frame = true;
    }

    if (truncated_bytes > 0) {
        auto const BUFFER_SIZE = _receive_buffer.size();
        crLogW("DefaultSink::onAfterGettingFrame() The input frame data was too large for our buffer size ({})"
               "{}bytes of trailing data was dropped!"
               "Correct this by increasing the 'bufferSize' parameter in the 'createNew()' call to at least {}",
               BUFFER_SIZE, truncated_bytes, BUFFER_SIZE + truncated_bytes);
    }

    // Write the input data to the file, with the start code in front:
    write(NAL_START_CODE, sizeof(NAL_START_CODE), presentation_time);
    write(_receive_buffer.data(), frame_size, presentation_time);

    if (isClosed()) {
        // The output file has closed.
        // Handle this the same way as if the input source had closed:
        if (fSource != nullptr) {
            fSource->stopGettingFrames();
        }
        onSourceClosure();
        return;
    }

    // Then continue, to request the next frame of data:
    continuePlaying();
}

Boolean DefaultSink::continuePlaying()
{
    if (fSource == nullptr) {
        return False;
    }

    // Request the next frame of data from our input source.
    // afterGettingFrame() will get called later, when it arrives:
    fSource->getNextFrame(_receive_buffer.data(), _receive_buffer.size(),
                          __after_getting_frame_cb, this,
                          __source_closure_cb, this);
    return True;
}

bool DefaultSink::isClosed() const
{
    return !_writer->isOpen();
}

unsigned DefaultSink::write(unsigned char const * data, unsigned size, struct timeval presentation_time)
{
//  // Special case: Open a new file on-the-fly for this frame
//  if (presentation_time.tv_usec == fPrevPresentationTime.tv_usec &&
//      presentation_time.tv_sec == fPrevPresentationTime.tv_sec) {
//      // The presentation time is unchanged from the previous frame, so we add a 'counter'
//      // suffix to the file name, to distinguish them:
//      sprintf(fPerFrameFileNameBuffer, "%s-%lu.%06lu-%u", fPerFrameFileNamePrefix,
//              presentation_time.tv_sec, presentation_time.tv_usec, ++fSamePresentationTimeCounter);
//  } else {
//      sprintf(fPerFrameFileNameBuffer, "%s-%lu.%06lu", fPerFrameFileNamePrefix,
//              presentation_time.tv_sec, presentation_time.tv_usec);
//      fPrevPresentationTime = presentation_time; // for next time
//      fSamePresentationTimeCounter = 0; // for next time
//  }
//  fOutFid = OpenOutputFile(envir(), fPerFrameFileNameBuffer);

//#ifdef TEST_LOSS
//  static unsigned const framesPerPacket = 10;
//  static unsigned const frameCount = 0;
//  static Boolean const packetIsLost;
//  if ((frameCount++)%framesPerPacket == 0) {
//    packetIsLost = (our_random()%10 == 0); // simulate 10% packet loss #####
//  }
//#endif

    _writer->write(data, size, presentation_time);
    return 0;
}

} // namespace client
} // namespace libc2rtsp

