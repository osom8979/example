/**
 * @file   Log.hpp
 * @brief  Log class prototype.
 * @author zer0
 * @date   2018-09-13
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_LOG_LOG_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_LOG_LOG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/Log.hpp>
#include <libc2rtsp/export.h>

namespace libc2rtsp {
namespace log       {

TBAG_CONSTEXPR char const * const DEFAULT_LOGGER_NAME = "cr"; // 'C'yclops-cloud 'R'tsp library.

using Logger = libtbag::log::Logger;
using Severity = libtbag::log::Severity;

// @formatter:off
Severity const       OFF_SEVERITY = ::libtbag::log::OFF_SEVERITY;
Severity const EMERGENCY_SEVERITY = ::libtbag::log::EMERGENCY_SEVERITY;
Severity const     ALERT_SEVERITY = ::libtbag::log::ALERT_SEVERITY;
Severity const  CRITICAL_SEVERITY = ::libtbag::log::CRITICAL_SEVERITY;
Severity const     ERROR_SEVERITY = ::libtbag::log::ERROR_SEVERITY;
Severity const   WARNING_SEVERITY = ::libtbag::log::WARNING_SEVERITY;
Severity const    NOTICE_SEVERITY = ::libtbag::log::NOTICE_SEVERITY;
Severity const      INFO_SEVERITY = ::libtbag::log::INFO_SEVERITY;
Severity const     DEBUG_SEVERITY = ::libtbag::log::DEBUG_SEVERITY;
// @formatter:on

C2RTSP_API Logger * createColorStdoutLogger();
C2RTSP_API Logger * createRawStdoutLogger();
C2RTSP_API Logger * createStdoutLogger();

C2RTSP_API Logger * getLogger();
C2RTSP_API bool removeLogger();

C2RTSP_API void setLevel(int level);
C2RTSP_API void setSeverity(Severity level);

} // namespace log
} // namespace libc2rtsp

#define crLogM(...)  tLogM(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogA(...)  tLogA(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogC(...)  tLogC(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogE(...)  tLogE(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogW(...)  tLogW(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogN(...)  tLogN(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogI(...)  tLogI(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogD(...)  tLogD(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)

#define crLogIfM(c, ...)  tLogIfM(c, ::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogIfA(c, ...)  tLogIfA(c, ::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogIfC(c, ...)  tLogIfC(c, ::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogIfE(c, ...)  tLogIfE(c, ::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogIfW(c, ...)  tLogIfW(c, ::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogIfN(c, ...)  tLogIfN(c, ::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogIfI(c, ...)  tLogIfI(c, ::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogIfD(c, ...)  tLogIfD(c, ::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)

#define crLogSM(...)  tLogSM(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogSA(...)  tLogSA(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogSC(...)  tLogSC(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogSE(...)  tLogSE(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogSW(...)  tLogSW(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogSN(...)  tLogSN(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogSI(...)  tLogSI(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define crLogSD(...)  tLogSD(::libc2rtsp::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_LOG_LOG_HPP__

