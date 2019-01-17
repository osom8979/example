/**
 * @file   Reader.hpp
 * @brief  Reader class prototype.
 * @author zer0
 * @date   2018-10-10
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_SOURCE_READER_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_SOURCE_READER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libc2rtsp/export.h>

#ifndef _NET_COMMON_H
#include <NetCommon.h> // 'timeval' structure
#endif

namespace libc2rtsp {
namespace source    {

/**
 * Reader class prototype.
 *
 * @author zer0
 * @date   2018-10-10
 */
class C2RTSP_API Reader : private libtbag::Noncopyable
{
public:
    using Err = libtbag::Err;

public:
    Reader();
    virtual ~Reader();

public:
    virtual bool open(char const * path) = 0;
    virtual void close() = 0;

public:
    virtual bool isOpen() const = 0;

public:
    virtual Err read(unsigned char const * data,
                     unsigned size,
                     unsigned * read_size = nullptr,
                     struct timeval * presentation_time = nullptr) = 0;
};

} // namespace source
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_SOURCE_READER_HPP__

