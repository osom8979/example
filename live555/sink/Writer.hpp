/**
 * @file   Writer.hpp
 * @brief  Writer class prototype.
 * @author zer0
 * @date   2018-10-10
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_SINK_WRITER_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_SINK_WRITER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libc2rtsp/export.h>

#ifndef _NET_COMMON_H
#include <NetCommon.h> // 'timeval' structure
#endif

namespace libc2rtsp {
namespace sink      {

/**
 * Writer class prototype.
 *
 * @author zer0
 * @date   2018-10-10
 */
class C2RTSP_API Writer : private libtbag::Noncopyable
{
public:
    Writer();
    virtual ~Writer();

public:
    virtual bool open(char const * path) = 0;
    virtual void close() = 0;

public:
    virtual bool isOpen() const = 0;

public:
    virtual unsigned write(unsigned char const * data, unsigned size, struct timeval presentation_time) = 0;
};

} // namespace sink
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_SINK_WRITER_HPP__

