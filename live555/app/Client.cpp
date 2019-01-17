/**
 * @file   Client.cpp
 * @brief  Client class implementation.
 * @author zer0
 * @date   2018-09-12
 * @date   2018-09-27 (Rename: Rtsp -> Client)
 */

#include <libc2rtsp/app/Client.hpp>
#include <libc2rtsp/log/Log.hpp>
#include <libc2rtsp/rtsp/Environment.hpp>
#include <libc2rtsp/client/StreamClient.hpp>

#include <libtbag/filesystem/Path.hpp>

#include <cstdlib>
#include <cassert>

namespace libc2rtsp {
namespace app       {

Client::Client(Options const & options) : _options(options), _scheduler(nullptr), _environment(nullptr), _exit_watch(0)
{
    // EMPTY.
}

Client::~Client()
{
    // EMPTY.
}

int Client::run()
{
    if (_options.rtsp_url.empty()) {
        crLogE("Client::run() The RTSP URL is empty.");
        return EXIT_FAILURE;
    }

    _scheduler = BasicTaskScheduler::createNew();
    _environment = libc2rtsp::rtsp::Environment::createNew(*_scheduler, _options.print_verbose);

    using namespace libtbag::filesystem;
    using namespace libc2rtsp::client;
    try {
        StreamClient::Params params;
        params.environment = _environment;
        params.rtsp_url = _options.rtsp_url;
        params.verbosity_level = _options.print_verbose ? 1 : 0;
        params.application_name = Path::getExePath().getName();
        params.request_streaming_over_tcp = _options.rtp_tcp;
        params.sink_url = _options.resource_url;

        StreamClient * client = new StreamClient(params);
        assert(client != nullptr);
        client->sendDescribe();
    } catch (...) {
        crLogE("Client::run() Failed to create a RTSP client for URL({}): {}",
               _options.rtsp_url, _environment->getResultMsg());
        return EXIT_FAILURE;
    }

    _environment->taskScheduler().doEventLoop(&_exit_watch);

    assert(_environment != nullptr);
    // We delete ourselves only if we have no remaining state:
    if (_environment->reclaim() == False) {
        assert(false && "LiveMedia is NULL or GroupSock is NULL");
    }
    _environment = nullptr;

    assert(_scheduler != nullptr);
    delete _scheduler;
    _scheduler = nullptr;

    return EXIT_SUCCESS;
}

} // namespace app
} // namespace libc2rtsp

