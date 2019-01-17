/**
 * @file   Options.hpp
 * @brief  Options class prototype.
 * @author zer0
 * @date   2018-09-12
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_UTIL_OPTIONS_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_UTIL_OPTIONS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>

namespace libc2rtsp {
namespace util      {

TBAG_CONSTEXPR int const DEFAULT_LOG_LEVEL = 5; // 5 is WARNING_SEVERITY

struct Options
{
    bool print_help    = false;
    bool print_verbose = false;
    bool print_version = false;
    bool print_unknown = false;

    bool log_raw   = false;
    bool log_color = false;
    int  log_level = DEFAULT_LOG_LEVEL;

    bool rtp_tcp = false;

    std::string rtsp_url;
    std::string resource_url;
};

} // namespace util
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_UTIL_OPTIONS_HPP__

