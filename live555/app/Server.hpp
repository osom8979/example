/**
 * @file   Server.hpp
 * @brief  Server class prototype.
 * @author zer0
 * @date   2018-09-27
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_APP_SERVER_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_APP_SERVER_HPP__

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
 * Server class prototype.
 *
 * @author zer0
 * @date   2018-09-27
 */
class C2RTSP_API Server : private libtbag::Noncopyable
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
    Server(Options const & options);
    ~Server();

public:
    int run();
};

} // namespace app
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_APP_SERVER_HPP__

