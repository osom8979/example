/**
 * @file   Main.cpp
 * @brief  Main class implementation.
 * @author zer0
 * @date   2018-09-10
 */

#include "Main.hpp"
#include <libtbag/filesystem/Path.hpp>

#include <cstdlib>
#include <iostream>

namespace main {

int getMajorVersion()
{
#if defined(MAJOR_VERSION)
    return MAJOR_VERSION;
#else
    return 0;
#endif
}

int getMinorVersion()
{
#if defined(MINOR_VERSION)
    return MINOR_VERSION;
#else
    return 0;
#endif
}

Main::Main(int argc, char ** argv, char ** envs)
{
    using namespace libtbag::filesystem;
    _cmd.setSynopsis(std::string("Usage: ") + Path::getExePath().getName() + " {options} commands\n");
    _cmd.setRemarks("\nSimple entry-point class demo");
    _cmd.insertHelpCommand([&](Arguments const & args){
        _options.print_help = true;
    });
    _cmd.insertDefault("verbose", &_options.print_verbose, true, "Be more verbose/talkative during the operation.");
    _cmd.insertDefault("version", &_options.print_version, true, "print the version number and exit.");
    _cmd.setDefaultCallback([&](Arguments const & args){
        if (!args.getName().empty()) {
            // This block comes when an unknown option is hit.
            _options.print_unknown = true;
            return;
        }
        if (!args.empty()) {
            // Command arguments.
            _commands.push_back(args.getOriginalArgumentString());
        }
    });
    _cmd.request(argc, argv);
    if (_commands.empty()) {
        _options.print_unknown = true;
    }
}

Main::~Main()
{
    // EMPTY.
}

int Main::run()
{
    if (_options.print_help) {
        std::cout << _cmd.help() << std::endl;
        return EXIT_SUCCESS;
    }
    if (_options.print_version) {
        std::cout << getMajorVersion() << "." << getMinorVersion() << std::endl;
        return EXIT_SUCCESS;
    }
    if (_options.print_unknown) {
        std::cerr << "Unknown command." << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

} // namespace main

