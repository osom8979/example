/**
 * @file   ServerSubsession.hpp
 * @brief  ServerSubsession class prototype.
 * @author zer0
 * @date   2018-09-27
 * @date   2018-10-04 (Rename: H264MediaSubsession -> ServerSubsession)
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_SERVERSUBSESSION_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_SERVERSUBSESSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libc2rtsp/export.h>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

#include <string>

namespace libc2rtsp {
namespace server    {

/**
 * ServerSubsession class prototype.
 *
 * @author zer0
 * @date   2018-09-27
 * @date   2018-10-04 (Rename: H264MediaSubsession -> ServerSubsession)
 */
class C2RTSP_API ServerSubsession : public OnDemandServerMediaSubsession
{
public:
    TBAG_CONSTEXPR static int const SDP_CHECK_DELAY_MICROSEC = 100000;
    TBAG_CONSTEXPR static unsigned const DEFAULT_ESTIMATED_BITRATE_KBPS = 500;

private:
    std::string _source_url;

private:
    int         _sdp_check_delay;
    char        _sdp_done;  ///< Used when setting up "AUX SDP Line"
    std::string _sdp_line;  ///< AUX SDP Line.

private:
    RTPSink * _dummy_sink;

protected:
    ServerSubsession(UsageEnvironment & env,
                     Boolean reuse_first_source,
                     std::string const & source_url);
    virtual ~ServerSubsession();

public:
    static ServerSubsession * createNew(UsageEnvironment & env,
                                        Boolean reuse_first_source,
                                        std::string const & source_url);

protected:
    inline void setSdpDoneFlag() TBAG_NOEXCEPT
    { _sdp_done = ~0; }

public:
    C2RTSP_API friend void __check_for_aux_sdp_line_cb(void * client_Data);
    C2RTSP_API friend void __after_playing_dummy_cb(void * client_data);

protected:
    /** Check whether the sink's 'auxSDPLine()' is ready */
    virtual void onCheckForAuxSDPLine();
    virtual void onAfterPlayingDummy();

protected:
//    /**
//     * This routine is used to seek by relative (i.e., NPT) time.
//     *
//     * @param[in] input_source
//     *      Frame source.
//     * @param[in] seek_npt
//     *      Seek NPT.
//     * @param[in] stream_duration
//     *      If  > 0.0, specifies how much data to stream, past <code>seek_npt</code>. @n
//     *      If <= 0.0, all remaining data is streamed.
//     * @param[out] stream_size
//     *      returns the size (in bytes) of the data to be streamed, @n
//     *      or 0 if unknown or unlimited.
//     */
//    virtual void seekStreamSource(FramedSource * input_source,
//                                  double & seek_npt,
//                                  double stream_duration,
//                                  u_int64_t & stream_size) override;

//    /**
//     * This routine is used to seek by 'absolute' time.
//     *
//     * @param[in] input_source
//     * @param[out] abs_start
//     *      should be a string of the form "YYYYMMDDTHHMMSSZ" or "YYYYMMDDTHHMMSS.<frac>Z".
//     * @param[out] abs_end
//     *      should be either NULL (for no end time), @N
//     *      or a string of the same form as <code>abs_start</code>.
//     */
//    virtual void seekStreamSource(FramedSource * input_source,
//                                  char* & abs_start,
//                                  char* & abs_end) override;

protected:
    virtual char const * getAuxSDPLine(RTPSink * rtp_sink, FramedSource * input_source) override;

    /**
     * Create frame source.
     *
     * @param client_session_id
     *      Client session ID.
     * @param[out] est_bitrate
     *      Stream's estimated bitrate, in kbps
     *
     * @return
     *  Created frame source.
     */
    virtual FramedSource * createNewStreamSource(unsigned client_session_id,
                                                 unsigned & estimated_bitrate) override;

protected:
    virtual RTPSink * createNewRTPSink(Groupsock * rtp_groupsock,
                                       unsigned char rtp_payload_type_if_dynamic,
                                       FramedSource * input_source) override;
};

} // namespace server
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_SERVERSUBSESSION_HPP__

