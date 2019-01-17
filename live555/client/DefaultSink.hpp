/**
 * @file   DefaultSink.hpp
 * @brief  DefaultSink class prototype.
 * @author zer0
 * @date   2018-09-13
 * @date   2018-10-01 (Rename: DummySink -> MemorySink)
 * @date   2018-10-02 (Rename: MemorySink -> DefaultSink)
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_DEFAULTSINK_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_DEFAULTSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libc2rtsp/export.h>
#include <libc2rtsp/sink/Writer.hpp>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

#include <string>
#include <vector>
#include <memory>

namespace libc2rtsp {
namespace client    {

/**
 * DefaultSink class prototype.
 *
 * @author zer0
 * @date   2018-09-13
 * @date   2018-10-01 (Rename: DummySink -> MemorySink)
 * @date   2018-10-02 (Rename: MemorySink -> DefaultSink)
 *
 * @remarks
 *  Define a data sink (a subclass of "MediaSink") to receive the data for each subsession
 *  (i.e., each audio or video 'substream').
 *
 *  In practice, this might be a class (or a chain of classes)
 *  that decodes and then renders the incoming audio or video.
 *
 *  Or it might be a "FileSink", for outputting the received data into a file
 *  (as is done by the "openRTSP" application).
 *
 *  In this example code, however, we define a simple 'dummy' sink that receives incoming data,
 *  but does nothing with it.
 */
class C2RTSP_API DefaultSink : public MediaSink
{
public:
    using ReceiveBuffer = std::vector<u_int8_t>;
    using Writer        = libc2rtsp::sink::Writer;
    using SharedWriter  = std::shared_ptr<Writer>;

public:
    /**
     * Even though we're not going to be doing anything with the incoming data,
     * we still need to receive it.
     */
    TBAG_CONSTEXPR static std::size_t const RECEIVE_BUFFER_SIZE = 1024 * 100;

private:
    MediaSubsession & _subsession;

private:
    ReceiveBuffer _receive_buffer;
    std::string   _stream_id;
    SharedWriter  _writer;

private:
    bool _verbose = true;

private:
    /**
     * SDP - H.264 sprop-parameter-sets
     *
     * @remarks
     *  sprop-parameter-sets contains information about sequence and picture parameters set for the incoming NAL stream.
     *  The value are Base64 encoded. A comma is used to separate any pair of parameter set in the list.
     *  For example,
     *  @code
     *   sprop-parameter-sets=Z2ASEW12 , AsaR3QD
     *  @endcode
     *
     *  In general, sprop-parameter-sets are the SPS and PPS of NAL unit needed to initialize the decoder.
     *  If you want to parse the sprop-parameter-sets, they are separated by a comma.
     *  The left hand side of the comma is SPS and the right hand side of the comma is PPS.
     */
    std::vector<std::string> _sprop_parameter_sets;
    bool _have_written_first_frame = false;

protected:
    /**
     * @param env        Environment object.
     * @param subsession identifies the kind of data that's being received.
     * @param streamId   identifies the stream itself.
     */
    DefaultSink(UsageEnvironment & env,
                MediaSubsession & subsession,
                char const * sink_url,
                char const * stream_id);
    virtual ~DefaultSink();

public:
    static DefaultSink * createNew(UsageEnvironment & env,
                                   MediaSubsession & subsession,
                                   char const * sink_url,
                                   char const * stream_id = nullptr);

public:
    inline bool isVerbose() const TBAG_NOEXCEPT { return _verbose; }

private:
    // @formatter:off
    C2RTSP_API friend void __after_getting_frame_cb(void * client_data, unsigned frame_size,
                                                    unsigned truncated_bytes,
                                                    struct timeval presentation_time,
                                                    unsigned durationInMicroseconds);
    C2RTSP_API friend void __source_closure_cb(void * client_data);
    // @formatter:on

public:
    std::string getFrameInfo(unsigned frame_size,
                             unsigned truncated_bytes,
                             struct timeval const & presentation_time);

protected:
    void onAfterGettingFrame(unsigned frame_size,
                             unsigned truncated_bytes,
                             struct timeval const & presentation_time,
                             unsigned duration_in_microseconds);

protected:
    virtual Boolean continuePlaying() override;

protected:
    bool isClosed() const;
    unsigned write(unsigned char const * data, unsigned size, struct timeval presentation_time);
};

} // namespace client
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_DEFAULTSINK_HPP__

