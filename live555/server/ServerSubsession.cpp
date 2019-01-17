/**
 * @file   ServerSubsession.cpp
 * @brief  ServerSubsession class implementation.
 * @author zer0
 * @date   2018-09-27
 * @date   2018-10-04 (Rename: H264MediaSubsession -> ServerSubsession)
 */

#include <libc2rtsp/server/ServerSubsession.hpp>
#include <libc2rtsp/server/DefaultSource.hpp>
#include <libc2rtsp/log/Log.hpp>

#include <H264VideoRTPSink.hh>
#include <ByteStreamFileSource.hh>
#include <H264VideoStreamFramer.hh>

namespace libc2rtsp {
namespace server    {

ServerSubsession::ServerSubsession(UsageEnvironment & env, Boolean reuse_first_source, std::string const & source_url)
        : OnDemandServerMediaSubsession(env, reuse_first_source),
          _source_url(source_url), _sdp_check_delay(SDP_CHECK_DELAY_MICROSEC), _sdp_done(), _sdp_line(),
          _dummy_sink(nullptr)
{
    // EMPTY.
}

ServerSubsession::~ServerSubsession()
{
    // EMPTY.
}

ServerSubsession * ServerSubsession::createNew(UsageEnvironment & env, Boolean reuse_first_source, std::string const & source_url)
{
    return new ServerSubsession(env, reuse_first_source, source_url);
}

void __check_for_aux_sdp_line_cb(void * client_Data)
{
    ServerSubsession * subsession = (ServerSubsession*)client_Data;
    assert(subsession != nullptr);
    subsession->onCheckForAuxSDPLine();
}

void __after_playing_dummy_cb(void * client_data)
{
    ServerSubsession * subsession = (ServerSubsession*)client_data;
    assert(subsession);
    subsession->onAfterPlayingDummy();
}

void ServerSubsession::onAfterPlayingDummy()
{
    envir().taskScheduler().unscheduleDelayedTask(nextTask());
    setSdpDoneFlag();
}

void ServerSubsession::onCheckForAuxSDPLine()
{
    nextTask() = nullptr;

    char const * spd_line = nullptr;

    if (!_sdp_line.empty()) {
        setSdpDoneFlag();

    } else if (_dummy_sink != nullptr && (spd_line = _dummy_sink->auxSDPLine()) != nullptr) {
        _sdp_line = std::string(spd_line);
        _dummy_sink = nullptr;
        setSdpDoneFlag();

    } else if (!_sdp_done) {
        nextTask() = envir().taskScheduler().scheduleDelayedTask(_sdp_check_delay, __check_for_aux_sdp_line_cb, this);
    }
}

char const * ServerSubsession::getAuxSDPLine(RTPSink * rtp_sink, FramedSource * input_source)
{
    if (!_sdp_line.empty()) {
        // it's already been set up (for a previous client)
        return _sdp_line.c_str();
    }

    if (_dummy_sink == nullptr) {
        // we're not already setting it up for another, concurrent stream
        // isn't known until we start reading the file.
        //
        // Note: For H264 video files, the 'config' information ("profile-level-id" and "sprop-parameter-sets")
        //
        // This means that "rtp_sink"s "auxSDPLine()" will be nullptr initially,
        // and we need to start reading data from our file until this changes.
        _dummy_sink = rtp_sink;
        _dummy_sink->startPlaying(*input_source, __after_playing_dummy_cb, this);
        __check_for_aux_sdp_line_cb(this);
    }

    envir().taskScheduler().doEventLoop(&_sdp_done);
    return _sdp_line.c_str();
}

FramedSource * ServerSubsession::createNewStreamSource(unsigned client_session_id, unsigned & estimated_bitrate)
{
    estimated_bitrate = DEFAULT_ESTIMATED_BITRATE_KBPS;

    FramedSource * source = libc2rtsp::server::DefaultSource::createNew(envir(), _source_url.c_str());
    if (source == nullptr) {
        return nullptr;
    } else {
        crLogI("ServerSubsession::createNewStreamSource() Create source: {}", _source_url);
    }

    return H264VideoStreamFramer::createNew(envir(), source);
}

RTPSink * ServerSubsession::createNewRTPSink(Groupsock * rtp_groupsock,
                                             unsigned char rtp_payload_type_if_dynamic,
                                             FramedSource * input_source)
{
    return H264VideoRTPSink::createNew(envir(), rtp_groupsock, rtp_payload_type_if_dynamic);
}

} // namespace server
} // namespace libc2rtsp

