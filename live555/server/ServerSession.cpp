/**
 * @file   ServerSession.cpp
 * @brief  ServerSession class implementation.
 * @author zer0
 * @date   2018-10-04
 */

#include <libc2rtsp/server/ServerSession.hpp>

namespace libc2rtsp {
namespace server    {

ServerSession::ServerSession(UsageEnvironment & env,
                             char const * stream_name,
                             char const * info,
                             char const * description,
                             Boolean is_ssm,
                             char const * misc_sdp_lines)
        : ServerMediaSession(env, stream_name, info, description, is_ssm, misc_sdp_lines)
{
    // EMPTY.
}

ServerSession::~ServerSession()
{
    // EMPTY.
}

ServerSession * ServerSession::createNew(UsageEnvironment & env,
                                         char const * stream_name,
                                         char const * info,
                                         char const * description,
                                         Boolean is_ssm,
                                         char const * misc_sdp_lines)
{
    return new ServerSession(env, stream_name, info, description, is_ssm, misc_sdp_lines);
}

} // namespace server
} // namespace libc2rtsp

