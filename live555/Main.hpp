/**
 * @file   Main.hpp
 * @brief  Main class prototype.
 * @author zer0
 * @date   2018-09-10
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_MAIN_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_MAIN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/string/HelpCommander.hpp>

#include <libc2rtsp/export.h>
#include <libc2rtsp/util/Options.hpp>

#include <vector>
#include <string>

namespace libc2rtsp {

C2RTSP_API int getMajorVersion();
C2RTSP_API int getMinorVersion();

/**
 * Main class prototype.
 *
 * @author zer0
 * @date   2018-09-10
 */
class C2RTSP_API Main : private libtbag::Noncopyable
{
public:
    using HelpCommander = libtbag::string::HelpCommander;
    using Arguments = libtbag::string::Arguments;
    using Commands = std::vector<std::string>;
    using Options = libc2rtsp::util::Options;

private:
    Options _options;

private:
    HelpCommander _help;
    Commands _commands;

private:
    Main();

public:
    Main(int argc, char ** argv, char ** envs);
    Main(std::string const & args);
    ~Main();

public:
    int run();
};

} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_MAIN_HPP__

