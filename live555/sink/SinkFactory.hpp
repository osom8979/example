/**
 * @file   SinkFactory.hpp
 * @brief  SinkFactory class prototype.
 * @author zer0
 * @date   2018-10-10
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_SINK_SINKFACTORY_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_SINK_SINKFACTORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libc2rtsp/export.h>
#include <libc2rtsp/sink/Writer.hpp>

#include <string>
#include <memory>

namespace libc2rtsp {
namespace sink      {

/**
 * SinkFactory class prototype.
 *
 * @author zer0
 * @date   2018-10-10
 */
class C2RTSP_API SinkFactory : private libtbag::Noncopyable
{
public:
    using Writer       = libc2rtsp::sink::Writer;
    using SharedWriter = std::shared_ptr<Writer>;

public:
    SinkFactory();
    virtual ~SinkFactory();

public:
    SharedWriter gen(std::string const & url);
};

} // namespace sink
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_SINK_SINKFACTORY_HPP__

