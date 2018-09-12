/**
 * @file   Main.hpp
 * @brief  Main class prototype.
 * @author zer0
 * @date   2018-09-10
 */

#ifndef __INCLUDE_PROJECT_MAIN_HPP__
#define __INCLUDE_PROJECT_MAIN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/string/HelpCommander.hpp>

//#include <export.h>

#include <vector>
#include <string>

namespace main {

/*EXPORT_API*/ int getMajorVersion();
/*EXPORT_API*/ int getMinorVersion();

/**
 * Main class prototype.
 *
 * @author zer0
 * @date   2018-09-10
 *
 * @remarks
 *  @code{.cpp}
 *   return Main(argc, argv, envs).run();
 *  @endcode
 */
class /*EXPORT_API*/ Main : private libtbag::Noncopyable
{
public:
    using HelpCommander = libtbag::string::HelpCommander;
    using Arguments = libtbag::string::Arguments;
    using Commands = std::vector<std::string>;

public:
    struct Options
    {
        bool print_help    = false;
        bool print_verbose = false;
        bool print_version = false;
        bool print_unknown = false;
    };

private:
    Options _options;

private:
    HelpCommander _cmd;
    Commands _commands;

public:
    Main(int argc, char ** argv, char ** envs);
    ~Main();

public:
    int run();
};

} // namespace main

#endif // __INCLUDE_PROJECT_MAIN_HPP__

