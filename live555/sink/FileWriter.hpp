/**
 * @file   FileWriter.hpp
 * @brief  FileWriter class prototype.
 * @author zer0
 * @date   2018-10-10
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_SINK_FILEWRITER_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_SINK_FILEWRITER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>

#include <libc2rtsp/export.h>
#include <libc2rtsp/sink/Writer.hpp>

#include <string>

namespace libc2rtsp {
namespace sink      {

/**
 * FileWriter class prototype.
 *
 * @author zer0
 * @date   2018-10-10
 */
class C2RTSP_API FileWriter : public Writer
{
public:
    using Path = libtbag::filesystem::Path;
    using File = libtbag::filesystem::File;

private:
    Path _path;
    File _file;

public:
    FileWriter();
    FileWriter(char const * path);
    virtual ~FileWriter();

public:
    virtual bool open(char const * path) override;
    virtual void close() override;

public:
    virtual bool isOpen() const override;

public:
    virtual unsigned write(unsigned char const * data, unsigned size, struct timeval presentation_time) override;
};

} // namespace sink
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_SINK_FILEWRITER_HPP__

