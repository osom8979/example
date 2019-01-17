/**
 * @file   FileReader.hpp
 * @brief  FileReader class prototype.
 * @author zer0
 * @date   2018-10-10
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_SOURCE_FILEREADER_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_SOURCE_FILEREADER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>

#include <libc2rtsp/export.h>
#include <libc2rtsp/source/Reader.hpp>

namespace libc2rtsp {
namespace source    {

/**
 * FileReader class prototype.
 *
 * @author zer0
 * @date   2018-10-10
 */
class C2RTSP_API FileReader : public Reader
{
public:
    using Path = libtbag::filesystem::Path;
    using File = libtbag::filesystem::File;

private:
    Path _path;
    File _file;

public:
    FileReader();
    FileReader(char const * path);
    virtual ~FileReader();

public:
    virtual bool open(char const * path) override;
    virtual void close() override;

public:
    virtual bool isOpen() const override;

public:
    virtual Err read(unsigned char const * data,
                     unsigned size,
                     unsigned * read_size = nullptr,
                     struct timeval * presentation_time = nullptr) override;
};

} // namespace source
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_SOURCE_FILEREADER_HPP__

