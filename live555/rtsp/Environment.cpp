/**
 * @file   Environment.cpp
 * @brief  Environment class implementation.
 * @author zer0
 * @date   2018-09-13
 */

#include <libc2rtsp/rtsp/Environment.hpp>
#include <libc2rtsp/log/Log.hpp>

namespace libc2rtsp {
namespace rtsp      {

Environment::Environment(TaskScheduler & scheduler, bool enable_console)
        : BasicUsageEnvironment(scheduler), _enable_console(enable_console)
{
    // EMPTY.
}

Environment::~Environment()
{
    // EMPTY.
}

Environment * Environment::createNew(TaskScheduler & scheduler, bool enable_console)
{
    return new Environment(scheduler, enable_console);
}

Environment & Environment::operator<<(char const * str)
{
    if (_enable_console) {
        Parent::operator<<(str);
    }
    return *this;
}

Environment & Environment::operator<<(int i)
{
    if (_enable_console) {
        Parent::operator<<(i);
    }
    return *this;
}

Environment & Environment::operator<<(unsigned u)
{
    if (_enable_console) {
        Parent::operator<<(u);
    }
    return *this;
}

Environment & Environment::operator<<(double d)
{
    if (_enable_console) {
        Parent::operator<<(d);
    }
    return *this;
}

Environment & Environment::operator<<(void * p)
{
    if (_enable_console) {
        Parent::operator<<(p);
    }
    return *this;
}

} // namespace rtsp
} // namespace libc2rtsp

