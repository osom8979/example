/**
 * @file   Main.cpp
 * @brief  Main class implementation.
 * @author zer0
 * @date   2018-09-10
 */

#include <libc2rtsp/Main.hpp>
#include <libc2rtsp/log/Log.hpp>
#include <libc2rtsp/app/Client.hpp>
#include <libc2rtsp/app/Server.hpp>

#include <libtbag/libtbag.h>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <cstdlib>
#include <iostream>

namespace libc2rtsp {

int getMajorVersion()
{
#if defined(C2RTSP_MAJOR_VERSION)
    return C2RTSP_MAJOR_VERSION;
#else
    return 0;
#endif
}

int getMinorVersion()
{
#if defined(C2RTSP_MINOR_VERSION)
    return C2RTSP_MINOR_VERSION;
#else
    return 0;
#endif
}

// --------------------
// Main implementation.
// --------------------

TBAG_CONSTEXPR char const * const COMMAND_SERVER  = "server";
TBAG_CONSTEXPR char const * const HELP_MSG_SERVER = "RTSP streaming server";
TBAG_CONSTEXPR char const * const COMMAND_CLIENT  = "client";
TBAG_CONSTEXPR char const * const HELP_MSG_CLIENT = "RTSP receive client";

Main::Main()
{
    using namespace libtbag::filesystem;
    std::stringstream ss;
    ss << "\nCyclops cloud - streaming input library\n"
       << "\nCommands:"
       << "\n" << COMMAND_SERVER << "\t" << HELP_MSG_SERVER
       << "\n" << COMMAND_CLIENT << "\t" << HELP_MSG_CLIENT
       << "\n\nClient example:"
       << "\n  " << Path::getExePath().getName()
       << " --verbose --rtp_tcp --log_level=8 --log_color --resource_url=file://h264.out --rtsp_url=rtsp://0.0.0.0:8554/live.sdp client"
       << "\n\nServer example:"
       << "\n  " << Path::getExePath().getName()
       << " --verbose --rtp_tcp --log_level=8 --log_color --resource_url=file://h264.out server";

    tbInitialize();

    libtbag::signal::registerDefaultStdTerminateHandler();
    libtbag::signal::registerDefaultHandler();

    _help.setSynopsis(std::string("Usage: ") + Path::getExePath().getName() + " {options} commands\n");
    _help.setRemarks(ss.str());
    _help.insertHelpCommand([&](Arguments const & args){
        _options.print_help = true;
    });
    _help.insertDefault("verbose"     , &_options.print_verbose, true, "Be more verbose/talkative during the operation.");
    _help.insertDefault("version"     , &_options.print_version, true, "print the version number and exit.");
    _help.insertDefault("rtsp_url"    , &_options.rtsp_url, std::string(), "RTSP protocol url (e.g. rtsp://...).", "{url}");
    _help.insertDefault("resource_url", &_options.resource_url, std::string(), "Resource url (e.g. file://...).", "{url}");
    _help.insertDefault("log_raw"     , &_options.log_raw, true, "Raw log messages.");
    _help.insertDefault("log_color"   , &_options.log_color, true, "Coloring log messages.");
    _help.insertDefault("log_level"   , &_options.log_level, libc2rtsp::util::DEFAULT_LOG_LEVEL, "logging level (0:off ~ 8:debug)", "{level}");
    _help.insertDefault("rtp_tcp"     , &_options.rtp_tcp, true, "Enable RTP-over-TCP mode.");
    _help.setDefaultCallback([&](Arguments const & args){
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
}

Main::Main(int argc, char ** argv, char ** envs) : Main()
{
    _help.request(argc, argv);
}

Main::Main(std::string const & args) : Main()
{
    _help.request(args);
}

Main::~Main()
{
    libc2rtsp::log::removeLogger();
    tbRelease();
}

int Main::run()
{
    if (_options.print_help) {
        std::cout << _help.help() << std::endl;
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
    if (_commands.empty()) {
        std::cerr << "Empty command." << std::endl;
        return EXIT_FAILURE;
    }

    libc2rtsp::log::Logger * logger = nullptr;
    if (_options.log_raw) {
        logger = libc2rtsp::log::createRawStdoutLogger();
    } else if (_options.log_color) {
        logger = libc2rtsp::log::createColorStdoutLogger();
    } else {
        logger = libc2rtsp::log::createStdoutLogger();
    }
    if (logger != nullptr) {
        libc2rtsp::log::setLevel(_options.log_level);
    } else {
        std::cerr << "[WARNING] Logger(c2rtsp) creation failed." << std::endl;
    }

    std::string const COMMAND = _commands[0];
    if (COMMAND == std::string(COMMAND_SERVER)) {
        return libc2rtsp::app::Server(_options).run();
    } else if (COMMAND == std::string(COMMAND_CLIENT)) {
        return libc2rtsp::app::Client(_options).run();
    }

    std::cerr << "Not found registered command." << std::endl;
    return EXIT_FAILURE;
}

} // namespace libc2rtsp

