/**
 * @file   third.h
 * @brief  Third-party library header.
 * @author zer0
 * @date   2016-02-24
 */

#ifndef __INCLUDE_THIRD__THIRD_H__
#define __INCLUDE_THIRD__THIRD_H__

#if defined(_MSC_VER)
# if defined(EXPORT)
#  define __LIB_EXPORT__  __declspec(dllexport)
# else
#  define __LIB_EXPORT__  __declspec(dllimport)
# endif
#else
# define __LIB_EXPORT__
#endif

#if defined(__cplusplus)
extern "C" {
#endif

__LIB_EXPORT__ char const * const __third__();

#if defined(__cplusplus)
}
#endif

#endif // __INCLUDE_THIRD__THIRD_H__

