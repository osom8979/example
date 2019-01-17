/**
 * @file   libc2rtsp.h
 * @brief  libc2rtsp class prototype.
 * @author zer0
 * @date   2018-09-10
 * @date   2018-09-13 (Rename: libc2in -> libc2rtsp)
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_LIBC2RTSP_H__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_LIBC2RTSP_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libc2rtsp/export.h>

#if defined(__cplusplus)
extern "C" {
#endif

C2RTSP_API int c2rtspGetMajorVersion();
C2RTSP_API int c2rtspGetMinorVersion();

C2RTSP_API int c2rtspMain(int argc, char ** argv, char ** envs);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* __INCLUDE_LIBC2CORE__LIBC2RTSP_LIBC2RTSP_H__ */

