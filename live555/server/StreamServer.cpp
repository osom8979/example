/**
 * @file   StreamServer.cpp
 * @brief  StreamServer class implementation.
 * @author zer0
 * @date   2018-10-04
 */

#include <libc2rtsp/server/StreamServer.hpp>

namespace libc2rtsp {
namespace server    {

StreamServer::StreamServer(UsageEnvironment & env,
                           int socket,
                           Port port,
                           UserAuthenticationDatabase * auth_database,
                           unsigned reclamation_seconds)
        : RTSPServer(env, socket, port, auth_database, reclamation_seconds)
{
    // EMPTY.
}

StreamServer::~StreamServer()
{
    // EMPTY.
}

StreamServer * StreamServer::createNew(UsageEnvironment & env,
                                       Port port,
                                       UserAuthenticationDatabase * auth_database,
                                       unsigned reclamation_seconds)
{
    int socket = setUpOurSocket(env, port);
    if (socket == -1) {
        return nullptr;
    }
    return new StreamServer(env, socket, port, auth_database, reclamation_seconds);
}

} // namespace server
} // namespace libc2rtsp

