
#ifndef __INCLUDE_C2RTSP__C2RTSP_EXPORT_API_H__
#define __INCLUDE_C2RTSP__C2RTSP_EXPORT_API_H__

#if defined(__GNUC__)
#define __C2RTSP_GNUC_FULLVERSION \
    ( (__GNUC__            * 10000) \
    + (__GNUC_MINOR__      *   100) \
    + (__GNUC_PATCHLEVEL__ *     1) )
#else
#define __C2RTSP_GNUC_FULLVERSION 0
#endif

#if defined(WIN32) || defined(_WIN32) || \
    defined(WIN64) || defined(_WIN64) || \
    defined(WINCE) || defined(_WINCE) || \
    defined(__TOS_WIN__) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || \
    defined(__WINDOWS__)
#define __C2RTSP_PLATFORM_WINDOWS
#endif

#if defined(C2RTSP_EXPORT_API)
# if defined(__C2RTSP_PLATFORM_WINDOWS)
#  define C2RTSP_API __declspec(dllexport)
# elif __C2RTSP_GNUC_FULLVERSION >= 40000
#  define C2RTSP_API __attribute__((visibility("default")))
# else
#  define C2RTSP_API
# endif
#else /* defined(C2RTSP_EXPORT_API) */
# if defined(__C2RTSP_PLATFORM_WINDOWS)
#  define C2RTSP_API __declspec(dllimport)
# else
#  define C2RTSP_API
# endif
#endif /* defined(C2RTSP_EXPORT_API) */

#if defined(__cplusplus)
extern "C" {
#endif

/*EMPTY*/

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* __INCLUDE_C2RTSP__C2RTSP_EXPORT_API_H__ */
