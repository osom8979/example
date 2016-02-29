/**
 * @file   config.h
 * @brief  Program config header.
 * @author zer0
 * @date   2015-08-03
 */

#ifndef __INCLUDE_EXAMPLE_ASIO__CONFIG_H__
#define __INCLUDE_EXAMPLE_ASIO__CONFIG_H__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

// ----------------
// Windows os only.
// ----------------

#ifdef WINDOWS

#define WIN32_LEAN_AND_MEAN

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

// Minimum Platform version.
#ifndef WINVER
#define WINVER _WIN32_WINNT
#endif

// Minimum Internet Explorer version.
#ifndef _WIN32_IE
#define _WIN32_IE 0x0700
#endif

#endif // WINDOWS

#endif // __INCLUDE_EXAMPLE_ASIO__CONFIG_H__

