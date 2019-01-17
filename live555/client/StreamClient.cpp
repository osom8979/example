/**
 * @file   StreamClient.cpp
 * @brief  StreamClient class implementation.
 * @author zer0
 * @date   2018-10-02
 */

#include <libc2rtsp/client/StreamClient.hpp>
#include <libc2rtsp/client/DefaultSink.hpp>
#include <libc2rtsp/log/Log.hpp>

#include <libtbag/functional/ReleaseCallback.hpp>

#include <cassert>
#include <string>

namespace libc2rtsp {
namespace client    {

TBAG_CONSTEXPR static float const DEFAULT_PLAY_SCALE = 1.0f;

// Number of seconds extra to delay, after the stream's expected duration.
TBAG_CONSTEXPR static unsigned const DELAY_SLOP = 2;

static std::string __getSafetyResultString(char const * result_string, bool error_log = false)
{
    std::string message;
    if (result_string != nullptr) {
        message = std::string(result_string);
        delete [] result_string;
    } else if (error_log) {
        crLogW("getSafetyResultString() Empty result string");
    }
    return message;
}

/**
 * @remarks
 *  SDP description example:
 *  @code
 *   v=0
 *   o=- 3045340869485451261 1 IN IP4 172.17.0.2
 *   s=Session streamed with GStreamer
 *   i=rtsp-server
 *   t=0 0
 *   a=tool:GStreamer
 *   a=type:broadcast
 *   a=control:*
 *   a=range:npt=0-
 *   m=video 0 RTP/AVP 96
 *   c=IN IP4 0.0.0.0
 *   b=AS:2097
 *   a=rtpmap:96 H264/90000
 *   a=framerate:30
 *   a=fmtp:96 packetization-mode=1;profile-level-id=42c01f;sprop-parameter-sets=Z0LAH9oBQBbsBagICAoAAAMAAgAAAwB5HjBlQA==,aM48gA==
 *   a=control:stream=0
 *   a=ts-refclk:local
 *   a=mediaclk:sender
 *  @endcode
 */
void __continue_after_describe_cb(RTSPClient * client, int code, char * msg)
{
    auto * instance = (StreamClient*)client;
    assert(instance != nullptr);
    instance->onDescribe(code, __getSafetyResultString(msg));
}

void __continue_after_setup_cb(RTSPClient * client, int code, char * msg)
{
    auto * instance = (StreamClient*)client;
    assert(instance != nullptr);
    instance->onSetup(code, __getSafetyResultString(msg));
}

void __continue_after_play_cb(RTSPClient * client, int code, char * msg)
{
    auto * instance = (StreamClient*)client;
    assert(instance != nullptr);
    instance->onPlay(code, __getSafetyResultString(msg));
}

void __subsession_after_playing_cb(void * client_data)
{
    auto * subsession = (MediaSubsession*)client_data;
    assert(subsession != nullptr);

    auto * client = (StreamClient*)subsession->miscPtr;
    assert(client != nullptr);

    client->onSubsessionAfterPlaying(subsession);
}

void __subsession_bye_cb(void * client_data)
{
    auto * subsession = (MediaSubsession*)client_data;
    assert(subsession != nullptr);

    auto * client = (StreamClient*)subsession->miscPtr;
    assert(client != nullptr);

    client->onSubsessionBye(subsession);
}

void __stream_timer_cb(void * client_data)
{
    auto * client = (StreamClient*)client_data;
    assert(client != nullptr);

    client->onStreamTimer();
}

// ----------------------------
// StreamClient implementation.
// ----------------------------

StreamClient::StreamClient(Params const & params)
        : RTSPClient(*params.environment,
                     params.rtsp_url.c_str(),
                     params.verbosity_level,
                     params.application_name.c_str(),
                     params.tunnel_over_http_port_number,
                     params.socket_number_to_server),
          _params(params)
{
    // EMPTY.
}

StreamClient::~StreamClient()
{
    // EMPTY.
}

void StreamClient::sendDescribe()
{
    sendDescribeCommand(__continue_after_describe_cb);
}

void StreamClient::sendSetup(MediaSubsession & subsession)
{
    sendSetupCommand(subsession, __continue_after_setup_cb, false, _params.request_streaming_over_tcp);
}

void StreamClient::sendPlay(MediaSession & session, char const * abs_start_time, char const * abs_end_time)
{
    sendPlayCommand(session, __continue_after_play_cb, abs_start_time, abs_end_time, DEFAULT_PLAY_SCALE);
}

void StreamClient::sendPlay(MediaSession & session, double start, double end)
{
    sendPlayCommand(session, __continue_after_play_cb, start, end, DEFAULT_PLAY_SCALE);
}

void StreamClient::sendTeardown(MediaSession & session)
{
    // Don't bother handling the response to the "TEARDOWN".
    sendTeardownCommand(session, nullptr);
}

void StreamClient::onDescribe(int code, std::string const & message)
{
    using namespace libtbag::functional;
    ReleaseCallback shutdown_task([&](){
        // An unrecoverable error occurred with this stream.
        shutdownStream();
    });

    if (code != 0) {
        // Failed to get a SDP description
        crLogE("StreamClient::onDescribe() Error code({}) {}", code, message);
        return;
    }

    crLogIfI(_params.verbosity_level >= 2, "StreamClient::onDescribe() SDP description:\n{}", message);

    if (!_state.newSession(envir(), message)) {
        crLogE("StreamClient::onDescribe() Failed to create a MediaSession");
        return;
    }
    assert(static_cast<bool>(_state.session));

    if (!_state.hasSubsessions()) {
        crLogE("StreamClient::onDescribe() No subsessions.");
        return;
    }

    // Then, create and set up our data source objects for the session.
    // We do this by iterating over the session's 'subsessions',
    // calling "MediaSubsession::initiate()", and then sending a RTSP "SETUP" command, on each one.
    // (Each 'subsession' will have its own data source.)
    _state.prepareToSubsessionsSetup();

    setupNextSubsession();
    shutdown_task.cancel();
}

void StreamClient::onSetup(int code, std::string const & message)
{
    using namespace libtbag::functional;
    ReleaseCallback next_setup_task([&](){
        setupNextSubsession();
    });

    auto subsession = _state.getCurrentSettedSubsession();
    assert(static_cast<bool>(subsession));

    if (code != 0) {
        crLogE("StreamClient::onSetup({}) Failed to set up the {}/{} subsession: {}",
               code, subsession->mediumName(), subsession->codecName(), message);
        return;
    }

    crLogD("StreamClient::onSetup() Set up the {} subsession: client port {}",
           subsession->mediumName(), subsession->codecName(),
           StreamState::getPortRangeString(*subsession.get()));

    // Having successfully setup the subsession,
    // create a data sink for it, and call "startPlaying()" on it.
    // (This will prepare the data sink to receive data;
    // the actual flow of data from the client won't start happening until later,
    // after we've sent a RTSP "PLAY" command.)
    if (::strcmp(subsession->codecName(), "H264") == 0) {
        subsession->sink = DefaultSink::createNew(envir(), *subsession.get(), _params.sink_url.c_str(), url());
    } else {
        crLogE("StreamClient::onSetup() Unsupported subsession: {}/{}",
               subsession->mediumName(), subsession->codecName());
        return;
    }

    if (subsession->sink == nullptr) {
        crLogE("StreamClient::onSetup() Failed to create a data sink for the {}/{} subsession: {}",
               subsession->mediumName(), subsession->codecName(), envir().getResultMsg());
        return;
    }

    crLogD("StreamClient::onSetup() Created a data sink for the {}/{} subsession",
           subsession->mediumName(), subsession->codecName());

    // A hack to let subsession handler functions get the "RTSPClient" from the subsession.
    subsession->miscPtr = this;
    MediaSource & source = *(subsession->readSource());
    subsession->sink->startPlaying(source, __subsession_after_playing_cb, subsession.get());

    // Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
    if (subsession->rtcpInstance() != nullptr) {
        subsession->rtcpInstance()->setByeHandler(__subsession_bye_cb, subsession.get());
    }
}

void StreamClient::onPlay(int code, std::string const & message)
{
    if (code != 0) {
        crLogE("StreamClient::onPlay() Failed to start playing session: code({}) message({})",
               code, message);
        // An unrecoverable error occurred with this stream.
        shutdownStream();
        return;
    }

    // Set a timer to be handled at the end of the stream's expected duration.
    // (if the stream does not already signal its end using a RTCP "BYE").
    //
    // This is optional.
    //
    // If, instead, you want to keep the stream active
    // - e.g., so you can later 'seek' back within it and do another RTSP "PLAY" -
    // then you can omit this code.
    //
    // Alternatively, if you don't want to receive the entire stream,
    // you could set this timer for some shorter value.
    if (_state.duration > 0) {
        _state.duration += DELAY_SLOP;
        auto const SECONDS_TO_DELAY = (unsigned)(_state.duration * 1000000);
        _state.stream_timer_task = envir().taskScheduler().scheduleDelayedTask(SECONDS_TO_DELAY, (TaskFunc*)__stream_timer_cb, this);
    }

    if (_state.duration > 0) {
        crLogI("StreamClient::onPlay() Started playing session; for up to {} seconds ...", _state.duration);
    } else {
        crLogI("StreamClient::onPlay() Started playing session ...");
    }
}

void StreamClient::setupNextSubsession()
{
    auto subsession = _state.nextSetupSubsession();
    auto & session = _state.session;

    if (!subsession) {
        // We've finished setting up all of the subsessions.
        // Now, send a RTSP "PLAY" command to start the streaming:

        crLogI("StreamClient::setupNextSubsession() Send Play !!");

        if (session->absStartTime() != nullptr) {
            // Special case:
            // The stream is indexed by 'absolute' time,
            // so send an appropriate "PLAY" command:
            char const * START_TIME = session->absStartTime();
            char const *   END_TIME = session->absEndTime();
            sendPlay(*session, START_TIME, END_TIME);
        } else {
            double const START_TIME = session->playStartTime();
            double const   END_TIME = session->playEndTime();
            _state.duration = END_TIME - START_TIME;
            sendPlay(*session);
        }
        return;
    }

    if (!subsession->initiate()) {
        // Give up on this subsession; Go to the next one!
        crLogW("StreamClient::setupNextSubsession() Failed to initiate the {}/{} subsession: {}",
               subsession->mediumName(), subsession->codecName(),
               envir().getResultMsg());
        setupNextSubsession();
        return;
    }

    crLogD("StreamClient::setupNextSubsession() Initiated the {}/{} subsession: client port {}",
           subsession->mediumName(), subsession->codecName(),
           StreamState::getPortRangeString(*subsession.get()));

    // Continue setting up this subsession, by sending a RTSP "SETUP" command:
    sendSetup(*subsession.get());
}

// Used to shut down and close a stream (including its "RTSPClient" object):
void StreamClient::shutdownStream()
{
    // First, check whether any subsessions have still to be closed:
    if (_state.session != nullptr) {
        Boolean someSubsessionsWereActive = False;
        MediaSubsessionIterator iter(*_state.session);
        MediaSubsession * subsession = nullptr;

        while ((subsession = iter.next()) != nullptr) {
            if (subsession->sink != nullptr) {
                Medium::close(subsession->sink);
                subsession->sink = nullptr;

                if (subsession->rtcpInstance() != nullptr) {
                    subsession->rtcpInstance()->setByeHandler(nullptr, nullptr); // in case the server sends a RTCP "BYE" while handling "TEARDOWN"
                }
                someSubsessionsWereActive = True;
            }
        }

        if (someSubsessionsWereActive) {
            sendTeardown(*_state.session);
        }
    }

    crLogN("StreamClient::shutdownStream() Closing the stream.");
    Medium::close(this);
    // Note that this will also cause this stream's "StreamClientState" structure to get reclaimed.

//    if (--rtspClientCount == 0) {
//        // The final stream has ended, so exit the application now.
//        // (Of course, if you're embedding this code into your own application, you might want to comment this out,
//        // and replace it with "eventLoopWatchVariable = 1;", so that we leave the LIVE555 event loop, and continue running "main()".)
//        exit(exitCode);
//    }
    exit(0);
}

void StreamClient::onSubsessionAfterPlaying(MediaSubsession * subsession)
{
    // Begin by closing this subsession's stream:
    Medium::close(subsession->sink);
    subsession->sink = nullptr;

    // Next, check whether *all* subsessions' streams have now been closed:
    MediaSession & session = subsession->parentSession();
    MediaSubsessionIterator iter(session);
    while ((subsession = iter.next()) != nullptr) {
        if (subsession->sink != nullptr) {
            // This subsession is still active
            return;
        }
    }

    // All subsessions' streams have now been closed, so shutdown the client:
    shutdownStream();
}

void StreamClient::onSubsessionBye(MediaSubsession * subsession)
{
    crLogI("StreamClient::onSubsessionBye() Received RTCP 'BYE' on {}/{} subsession",
           subsession->mediumName(), subsession->codecName());

    // Now act as if the subsession had closed:
    onSubsessionAfterPlaying(subsession);
}

void StreamClient::onStreamTimer()
{
    crLogW("StreamClient::onStreamTimer() Timeout!");
    _state.stream_timer_task = nullptr;

    // Shut down the stream:
    shutdownStream();
}

} // namespace client
} // namespace libc2rtsp

