/**
 * @file   Environment.hpp
 * @brief  Environment class prototype.
 * @author zer0
 * @date   2018-09-13
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_RTSP_ENVIRONMENT_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_RTSP_ENVIRONMENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libc2rtsp/export.h>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

namespace libc2rtsp {
namespace rtsp      {

/**
 * Environment class prototype.
 *
 * @author zer0
 * @date   2018-09-13
 */
class C2RTSP_API Environment : public BasicUsageEnvironment
{
public:
    using Parent = BasicUsageEnvironment;

private:
    bool _enable_console;

protected:
    Environment(TaskScheduler & scheduler, bool enable_console);
    virtual ~Environment();

public:
    static Environment * createNew(TaskScheduler & scheduler, bool enable_console = true);

public:
    virtual Environment & operator<<(char const * str) override;
    virtual Environment & operator<<(int i) override;
    virtual Environment & operator<<(unsigned u) override;
    virtual Environment & operator<<(double d) override;
    virtual Environment & operator<<(void * p) override;
};

} // namespace rtsp
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_RTSP_ENVIRONMENT_HPP__

