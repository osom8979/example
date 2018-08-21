/// @file log.h
/// @brief Logging helper.
/// @author zer0
/// @date 2014-09-04

#ifndef __INCLUDE_WXUPDATE__LOG_H__
#define __INCLUDE_WXUPDATE__LOG_H__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/log.h>

#if defined (WIN32) || defined(_WIN32)
# include <windows.h>
# if defined(UNICODE) || defined(_UNICODE)
#  define _LOG(msg) OutputDebugStringW(msg)
# else
#  define _LOG(msg) OutputDebugStringA(msg)
# endif // UNICODE
#else
# define _LOG(msg) wxLogMessage(msg)
#endif // WIN32

#endif // __INCLUDE_WXUPDATE__LOG_H__
