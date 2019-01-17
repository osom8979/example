/**
 * @file   ClientSession.cpp
 * @brief  ClientSession class implementation.
 * @author zer0
 * @date   2018-10-02
 */

#include <libc2rtsp/client/ClientSession.hpp>
#include <libc2rtsp/client/ClientSubsession.hpp>
#include <libc2rtsp/log/Log.hpp>

#include <cassert>

namespace libc2rtsp {
namespace client    {

ClientSession::ClientSession(UsageEnvironment & env) : MediaSession(env)
{
    // EMPTY.
}

ClientSession::~ClientSession()
{
    // EMPTY.
}

ClientSession * ClientSession::createNew(UsageEnvironment & env, std::string const & sdp_description)
{
    ClientSession * new_session = nullptr;
    try {
        new_session = new ClientSession(env);
    } catch (...) {
        crLogSE("ClientSession::createNew() Could not create object.");
        return nullptr;
    }

    assert(new_session != nullptr);
    if (!new_session->initializeWithSDP(sdp_description.c_str())) {
        delete new_session;
        crLogSE("ClientSession::createNew() SDP error.");
        return nullptr;
    }
    return new_session;
}

MediaSubsession * ClientSession::createNewMediaSubsession()
{
    return new ClientSubsession(*this);
}

} // namespace client
} // namespace libc2rtsp

