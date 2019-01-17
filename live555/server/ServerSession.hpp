/**
 * @file   ServerSession.hpp
 * @brief  ServerSession class prototype.
 * @author zer0
 * @date   2018-10-04
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_SERVERSESSION_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_SERVERSESSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libc2rtsp/export.h>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

namespace libc2rtsp {
namespace server    {

/**
 * ServerSession class prototype.
 *
 * @author zer0
 * @date   2018-10-04
 */
class C2RTSP_API ServerSession : public ServerMediaSession
{
public:
    ServerSession(UsageEnvironment & env,
                  char const * stream_name,
                  char const * info,
                  char const * description,
                  Boolean is_ssm,
                  char const * misc_sdp_lines);
    virtual ~ServerSession();

public:
    static ServerSession * createNew(UsageEnvironment & env,
                                     char const * stream_name = nullptr,
                                     char const * info = nullptr,
                                     char const * description = nullptr,
                                     Boolean is_ssm = False,
                                     char const * misc_sdp_lines = nullptr);
};

} // namespace server
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_SERVERSESSION_HPP__

