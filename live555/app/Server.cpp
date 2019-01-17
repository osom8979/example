/**
 * @file   Server.cpp
 * @brief  Server class implementation.
 * @author zer0
 * @date   2018-09-27
 */

#include <libc2rtsp/app/Server.hpp>
#include <libc2rtsp/log/Log.hpp>
#include <libc2rtsp/rtsp/Environment.hpp>
#include <libc2rtsp/server/StreamServer.hpp>
#include <libc2rtsp/server/ServerSession.hpp>
#include <libc2rtsp/server/ServerSubsession.hpp>

#include <cstdlib>
#include <cassert>

namespace libc2rtsp {
namespace app       {

TBAG_CONSTEXPR static char const * const DEFAULT_STREAM_NAME = "live";

Server::Server(Options const & options) : _options(options), _scheduler(nullptr), _environment(nullptr), _exit_watch(0)
{
    // EMPTY.
}

Server::~Server()
{
    // EMPTY.
}

int Server::run()
{
    _scheduler = BasicTaskScheduler::createNew();
    _environment = libc2rtsp::rtsp::Environment::createNew(*_scheduler, _options.print_verbose);

    char const * descriptionString = "Session streamed by \"testOnDemandRTSPServer\"";

    // To make the second and subsequent client for each stream reuse the same
    // input stream as the first client (rather than playing the file from the
    // start for each client), change the following "False" to "True":
    Boolean reuseFirstSource = False;

    using namespace libtbag::filesystem;
    using namespace libc2rtsp::server;

    StreamServer * server = nullptr;
    ServerSession * session = nullptr;

    try {
        server = StreamServer::createNew(*_environment, 8554, nullptr);
        session = ServerSession::createNew(*_environment, DEFAULT_STREAM_NAME, DEFAULT_STREAM_NAME, descriptionString);

        session->addSubsession(ServerSubsession::createNew(*_environment, reuseFirstSource, _options.resource_url));
        server->addServerMediaSession(session);

        char * url_raw = server->rtspURL(session);
        std::string const URL(url_raw);
        delete [] url_raw;
        crLogN("Server::run() Play this stream using the URL: {}", URL);

    } catch (...) {
        crLogE("Server::run() Failed to create RTSP server: {}", _environment->getResultMsg());
        return EXIT_FAILURE;
    }

    _environment->taskScheduler().doEventLoop(&_exit_watch); // Does not return.

    assert(_environment != nullptr);
    // We delete ourselves only if we have no remaining state:
    if (_environment->reclaim() == False) {
        assert(false && "LiveMedia is NULL or GroupSock is NULL");
    }
    _environment = nullptr;

    assert(_scheduler != nullptr);
    delete _scheduler;
    _scheduler = nullptr;

    return EXIT_FAILURE;
}

} // namespace app
} // namespace libc2rtsp

