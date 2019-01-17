/**
 * @file   DefaultSource.hpp
 * @brief  DefaultSource class prototype.
 * @author zer0
 * @date   2018-10-01
 * @date   2018-10-04 (Rename: MemorySource -> DefaultSource)
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_DEFAULTSOURCE_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_DEFAULTSOURCE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libc2rtsp/export.h>
#include <libc2rtsp/source/Reader.hpp>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

#include <string>
#include <vector>
#include <memory>

namespace libc2rtsp {
namespace server    {

/**
 * DefaultSource class prototype.
 *
 * @author zer0
 * @date   2018-10-01
 * @date   2018-10-04 (Rename: MemorySource -> DefaultSource)
 */
class C2RTSP_API DefaultSource : public FramedSource
{
public:
    using Reader       = libc2rtsp::source::Reader;
    using SharedReader = std::shared_ptr<Reader>;

private:
    SharedReader _reader;

protected:
    DefaultSource(UsageEnvironment & env, char const * source_url);
    virtual ~DefaultSource();

public:
    static DefaultSource * createNew(UsageEnvironment & env, char const * source_url);

private:
    virtual void doGetNextFrame() override;
    virtual void doStopGettingFrames() override;
};

} // namespace server
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_SERVER_DEFAULTSOURCE_HPP__

