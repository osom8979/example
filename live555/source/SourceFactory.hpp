/**
 * @file   SourceFactory.hpp
 * @brief  SourceFactory class prototype.
 * @author zer0
 * @date   2018-10-10
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_SOURCE_SOURCEFACTORY_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_SOURCE_SOURCEFACTORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libc2rtsp/export.h>
#include <libc2rtsp/source/Reader.hpp>

#include <string>
#include <memory>

namespace libc2rtsp {
namespace source    {

/**
 * SourceFactory class prototype.
 *
 * @author zer0
 * @date   2018-10-10
 */
class C2RTSP_API SourceFactory : private libtbag::Noncopyable
{
public:
    using Reader       = libc2rtsp::source::Reader;
    using SharedReader = std::shared_ptr<Reader>;

public:
    SourceFactory();
    virtual ~SourceFactory();

public:
    SharedReader gen(std::string const & url);
};
} // namespace source
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_SOURCE_SOURCEFACTORY_HPP__

