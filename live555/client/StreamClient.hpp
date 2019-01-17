/**
 * @file   StreamClient.hpp
 * @brief  StreamClient class prototype.
 * @author zer0
 * @date   2018-10-02
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_STREAMCLIENT_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_STREAMCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/container/Pointer.hpp>
#include <libc2rtsp/export.h>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

#include <cassert>
#include <string>
#include <vector>
#include <memory>

namespace libc2rtsp {
namespace client    {

/**
 * RTSP StreamClient class prototype.
 *
 * @author zer0
 * @date   2018-09-12
 *
 * @remarks
 *  Single stream (i.e., just from a single URL, once),
 */
class C2RTSP_API StreamClient : public RTSPClient
{
public:
    using Parent = RTSPClient;

public:
    /**
     * Define a class to hold per-stream state that we maintain throughout each stream's lifetime:
     */
    struct StreamState
    {
        TBAG_CONSTEXPR static std::size_t const UNKNOWN_SUBSESSION_CURSOR = -1;

        using SharedSession = std::shared_ptr<MediaSession>;
        using SubsessionPtr = libtbag::container::Pointer<MediaSubsession>;
        using Subsessions   = std::vector<SubsessionPtr>;

        SharedSession  session;
        Subsessions    subsessions;
        std::size_t    subsession_setup_cursor = UNKNOWN_SUBSESSION_CURSOR;

        TaskToken stream_timer_task = nullptr;
        double    duration          = 0.0;

        StreamState()
        {
            // EMPTY.
        }

        ~StreamState()
        {
            session.reset();
        }

        /**
         * Create a media session object from this SDP description.
         */
        SharedSession createSession(UsageEnvironment & env, std::string const & sdp_description)
        {
            try {
                auto * new_session = MediaSession::createNew(env, sdp_description.c_str());
                assert(new_session != nullptr);

                return SharedSession(new_session, [&](MediaSession * delete_session){
                    auto & env = delete_session->envir();
                    env.taskScheduler().unscheduleDelayedTask(stream_timer_task);
                    Medium::close(delete_session);
                });
            } catch (...) {
                // Failed to create a MediaSession object from the SDP description
                // crLogE("StreamClient::onDescribe() Failed to create a MediaSession object: {}", env.getResultMsg());
            }
            return SharedSession();
        }

        bool newSession(UsageEnvironment & env, std::string const & sdp_description)
        {
            session = createSession(env, sdp_description);
            return static_cast<bool>(session);
        }

//        std::string getSubsessionString() const
//        {
//            return std::string(subsession->mediumName()) + "/" + std::string(subsession->codecName());
//        }

        /**
         * This session has no media subsessions (i.e., no "m=..." lines)
         */
        bool hasSubsessions() const
        {
            return session && session->hasSubsessions();
        }

        void prepareToSubsessionsSetup()
        {
            MediaSubsessionIterator iter(*session);
            MediaSubsession * subsession = nullptr;
            subsessions.clear();
            while ((subsession = iter.next()) != nullptr) {
                subsessions.emplace_back(subsession);
            }
            subsession_setup_cursor = UNKNOWN_SUBSESSION_CURSOR;
        }

        SubsessionPtr nextSetupSubsession()
        {
            if (subsession_setup_cursor == UNKNOWN_SUBSESSION_CURSOR) {
                subsession_setup_cursor = 0;
            } else {
                ++subsession_setup_cursor;
            }
            if (0 <= COMPARE_AND(subsession_setup_cursor) < subsessions.size()) {
                return subsessions[subsession_setup_cursor];
            }
            return SubsessionPtr();
        }

        SubsessionPtr getCurrentSettedSubsession()
        {
            return subsessions[subsession_setup_cursor];
        }

        static std::string getPortRangeString(MediaSubsession const & subsession)
        {
            auto const PORT = subsession.clientPortNum();
            auto const PORT_STR = std::to_string(PORT);
            if (subsession.rtcpIsMuxed()) {
                return PORT_STR;
            } else {
                return PORT_STR + '-' + std::to_string(PORT + 1);
            }
        }
    };

public:
    struct Params
    {
        UsageEnvironment * environment;

        /** This client name. */
        std::string application_name;

        /** RTSP URL. */
        std::string rtsp_url;

        /** SINK URL. */
        std::string sink_url;

        /**
         * If this value is 1,
         * Print verbose output from each "RTSPClient".
         */
        int verbosity_level = 1;

        /**
         * If non-zero,
         * We tunnel RTSP (and RTP) over a HTTP connection with the given port number,
         */
        portNumBits tunnel_over_http_port_number = 0;

        /**
         * If greater than or equal to 0,
         * Then it is the socket number of an already-existing TCP connection to the server.
         *
         * (In this case, <code>rtsp_url</code> must point to the socket's endpoint,
         * so that it can be accessed via the socket.)
         */
        int socket_number_to_server = -1;

        /**
         * By default, we request that the server stream its data using RTP/UDP.
         * If, instead, you want to request that the server stream via RTP-over-TCP,
         * change the following to True:
         */
        bool request_streaming_over_tcp = false;
    };

private:
    Params      _params;
    StreamState _state;

public:
    StreamClient(Params const & params);
    virtual ~StreamClient();

public:
    inline StreamState       & state()       TBAG_NOEXCEPT { return _state; }
    inline StreamState const & state() const TBAG_NOEXCEPT { return _state; }

public:
    void sendDescribe();
    void sendSetup(MediaSubsession & subsession);
    void sendPlay(MediaSession & session, char const * abs_start_time, char const * abs_end_time = nullptr);
    void sendPlay(MediaSession & session, double start = 0.0, double end = -1.0);
    void sendTeardown(MediaSession & session);

public:
    std::string toString() const
    { return std::string(Parent::url()); }

private:
    // @formatter:off
    C2RTSP_API friend void __continue_after_describe_cb(RTSPClient * client, int code, char * msg);
    C2RTSP_API friend void __continue_after_setup_cb   (RTSPClient * client, int code, char * msg);
    C2RTSP_API friend void __continue_after_play_cb    (RTSPClient * client, int code, char * msg);
    // @formatter:on

protected:
    // @formatter:off
    virtual void onDescribe(int code, std::string const & message);
    virtual void onSetup   (int code, std::string const & message);
    virtual void onPlay    (int code, std::string const & message);
    // @formatter:on

private:
    void setupNextSubsession();
    void shutdownStream();

private:
    C2RTSP_API friend void __subsession_after_playing_cb(void * client_data);
    C2RTSP_API friend void __subsession_bye_cb(void * client_data);

protected:
    virtual void onSubsessionAfterPlaying(MediaSubsession * subsession);
    virtual void onSubsessionBye(MediaSubsession * subsession);

private:
    C2RTSP_API friend void __stream_timer_cb(void * client_data);

protected:
    virtual void onStreamTimer();
};

} // namespace client
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_STREAMCLIENT_HPP__

