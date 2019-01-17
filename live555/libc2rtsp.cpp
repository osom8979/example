/**
 * @file   libc2rtsp.cpp
 * @brief  libc2rtsp class implementation.
 * @author zer0
 * @date   2018-09-10
 * @date   2018-09-13 (Rename: libc2in -> libc2rtsp)
 */

#include <libc2rtsp/libc2rtsp.h>
#include <libc2rtsp/Main.hpp>

int c2rtspGetMajorVersion()
{
    return libc2rtsp::getMajorVersion();
}

int c2rtspGetMinorVersion()
{
    return libc2rtsp::getMinorVersion();
}

int c2rtspMain(int argc, char ** argv, char ** envs)
{
    return libc2rtsp::Main(argc, argv, envs).run();
}

