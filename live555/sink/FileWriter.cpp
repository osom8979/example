/**
 * @file   FileWriter.cpp
 * @brief  FileWriter class implementation.
 * @author zer0
 * @date   2018-10-10
 */

#include <libc2rtsp/sink/FileWriter.hpp>
#include <libc2rtsp/log/Log.hpp>
#include <libtbag/type/TypeTable.hpp>

#include <cassert>

namespace libc2rtsp {
namespace sink      {

FileWriter::FileWriter()
{
    // EMPTY.
}

FileWriter::FileWriter(char const * path) : FileWriter()
{
    if (!open(path)) {
        throw std::bad_alloc();
    }
}

FileWriter::~FileWriter()
{
    if (isOpen()) {
        close();
    }
}

bool FileWriter::open(char const * path)
{
    _path.setString(std::string(path));
    if (_path.exists()) {
        crLogW("FileWriter::open() Exists file: {}", _path.toString());
        return false;
    }
    return _file.open(_path);
}

void FileWriter::close()
{
    if (!_file.close()) {
        crLogW("FileWriter::close() Close file error.");
    }
}

bool FileWriter::isOpen() const
{
    return _file.isOpen();
}

unsigned FileWriter::write(unsigned char const * data, unsigned size, struct timeval presentation_time)
{
    int const WRITE_RESULT = _file.write((char const *)data, (std::size_t)size);
    if (WRITE_RESULT >= 0) {
        assert(WRITE_RESULT <= libtbag::type::TypeInfo<unsigned>::maximum());
        crLogI("FileWriter::write({}) File write size: {}", size, WRITE_RESULT);
        return (unsigned)WRITE_RESULT;
    }
    crLogW("FileWriter::write() File write error.");
    return 0;
}

} // namespace sink
} // namespace libc2rtsp

