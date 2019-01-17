/**
 * @file   StreamServer.hpp
 * @brief  StreamServer class prototype.
 * @author zer0
 * @date   2018-10-04
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_STREAMSERVER_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_STREAMSERVER_HPP__

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
 * StreamServer class prototype.
 *
 * @author zer0
 * @date   2018-10-04
 */
class C2RTSP_API StreamServer : public RTSPServer
{
protected:
    StreamServer(UsageEnvironment & env,
                 int socket,
                 Port port,
                 UserAuthenticationDatabase * auth_database,
                 unsigned reclamation_seconds);
    virtual ~StreamServer();

public:
    static StreamServer * createNew(UsageEnvironment & env,
                                    Port port = 554,
                                    UserAuthenticationDatabase * auth_database = nullptr,
                                    unsigned reclamation_seconds = 65);
};

} // namespace server
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_STREAMSERVER_HPP__

