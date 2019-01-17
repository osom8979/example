/**
 * @file   Client.hpp
 * @brief  Client class prototype.
 * @author zer0
 * @date   2018-09-12
 * @date   2018-09-27 (Rename: Rtsp -> Client)
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_APP_CLIENT_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_APP_CLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libc2rtsp/export.h>
#include <libc2rtsp/util/Options.hpp>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

namespace libc2rtsp {
namespace app       {

/**
 * Client class prototype.
 *
 * @author zer0
 * @date   2018-09-12
 * @date   2018-09-27 (Rename: Client -> Client)
 */
class C2RTSP_API Client : private libtbag::Noncopyable
{
public:
    using Options = libc2rtsp::util::Options;
    using EventLoopWatch = char volatile;

private:
    Options _options;

private:
    TaskScheduler    * _scheduler;
    UsageEnvironment * _environment;
    EventLoopWatch     _exit_watch;

public:
    Client(Options const & options);
    ~Client();

public:
    int run();
};

} // namespace app
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_APP_CLIENT_HPP__

