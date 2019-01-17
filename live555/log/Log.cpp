/**
 * @file   Log.cpp
 * @brief  Log class implementation.
 * @author zer0
 * @date   2018-09-13
 */

#include <libc2rtsp/log/Log.hpp>

namespace libc2rtsp {
namespace log       {

TBAG_CONSTEXPR bool const MUTEX_FLAG = false;
TBAG_CONSTEXPR bool const FLUSH_FLAG = false;

Logger * createColorStdoutLogger()
{
    return libtbag::log::createStdoutLogger(DEFAULT_LOGGER_NAME,
                                            libtbag::log::MakeType::DEFAULT_COLOR,
                                            MUTEX_FLAG, FLUSH_FLAG);
}

Logger * createRawStdoutLogger()
{
    return libtbag::log::createStdoutLogger(DEFAULT_LOGGER_NAME,
                                            libtbag::log::MakeType::RAW,
                                            MUTEX_FLAG, FLUSH_FLAG);
}

Logger * createStdoutLogger()
{
    return libtbag::log::createStdoutLogger(DEFAULT_LOGGER_NAME,
                                            libtbag::log::MakeType::DEFAULT,
                                            MUTEX_FLAG, FLUSH_FLAG);
}

Logger * getLogger()
{
    return libtbag::log::getLogger(DEFAULT_LOGGER_NAME);
}

bool removeLogger()
{
    return libtbag::log::removeLogger(DEFAULT_LOGGER_NAME);
}

void setLevel(int level)
{
    libtbag::log::setLevel(DEFAULT_LOGGER_NAME, level);
}

void setSeverity(Severity level)
{
    libtbag::log::setSeverity(DEFAULT_LOGGER_NAME, level);
}

} // namespace log
} // namespace libc2rtsp

