/**
 * @file   FileReader.cpp
 * @brief  FileReader class implementation.
 * @author zer0
 * @date   2018-10-10
 */

#include <libc2rtsp/source/FileReader.hpp>
#include <libc2rtsp/log/Log.hpp>

#include <libtbag/type/TypeTable.hpp>
#include <libtbag/time/Time.hpp>

#include <cassert>

namespace libc2rtsp {
namespace source    {

FileReader::FileReader()
{
    // EMPTY.
}

FileReader::FileReader(char const * path) : FileReader()
{
    if (!open(path)) {
        throw std::bad_alloc();
    }
}

FileReader::~FileReader()
{
    if (isOpen()) {
        close();
    }
}

bool FileReader::open(char const * path)
{
    _path.setString(std::string(path));
    if (!_path.exists()) {
        crLogW("FileReader::open() Not found file: {}", _path.toString());
        return false;
    }
    return _file.open(_path, File::Flags().clear().rdonly());
}

void FileReader::close()
{
    if (!_file.close()) {
        crLogW("FileReader::close() Close file error.");
    }
}

bool FileReader::isOpen() const
{
    return _file.isOpen();
}

FileReader::Err FileReader::read(unsigned char const * data,
                                 unsigned size,
                                 unsigned * read_size,
                                 struct timeval * presentation_time)
{
    using namespace libtbag::type;
    if (!_file.isOpen()) {
        return Err::E_EOPEN;
    }
    if (_file.isEof()) {
        return Err::E_EOF;
    }

    unsigned request_size = 0;

    // Try to read as many bytes as will fit in the buffer provided
    auto const FILE_SIZE   = _file.getSize();
    auto const FILE_OFFSET = _file.offset();
    if (FILE_OFFSET + size > FILE_SIZE) {
        crLogIfW(FILE_SIZE - FILE_OFFSET > TypeInfo<unsigned>::maximum(),
                 "FileReader::read() The data is truncated: {}",
                 FILE_SIZE - FILE_OFFSET - TypeInfo<unsigned>::maximum());
        request_size = static_cast<unsigned>(FILE_SIZE - FILE_OFFSET);
    } else {
        request_size = size;
    }

    int const READ_RESULT = _file.read((char*)data, request_size);
    if (READ_RESULT < 0) {
        crLogW("FileReader::read() File read error.");
        return Err::E_RDERR;
    }

    // We don't know a specific play time duration for this data,
    // so just record the current time as being the 'presentation time':
    long sec(0), micro(0);
    if (presentation_time != nullptr && isSuccess(libtbag::time::getTimeOfDay(&sec, &micro))) {
        presentation_time->tv_sec  = sec;
        presentation_time->tv_usec = micro;
    }

    assert(READ_RESULT >= 0);
    if (read_size != nullptr) {
        *read_size = (unsigned)READ_RESULT;
    }
    return Err::E_SUCCESS;
}

} // namespace source
} // namespace libc2rtsp

