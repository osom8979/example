/**
 * @file   ClientSubsession.cpp
 * @brief  ClientSubsession class implementation.
 * @author zer0
 * @date   2018-10-02
 */

#include <libc2rtsp/client/ClientSubsession.hpp>
#include <libc2rtsp/client/ClientSession.hpp>

namespace libc2rtsp {
namespace client    {

ClientSubsession::ClientSubsession(ClientSession & parent)
        : MediaSubsession((MediaSession&)parent)
{
    // EMPTY.
}

ClientSubsession::~ClientSubsession()
{
    // EMPTY.
}

std::string ClientSubsession::name() const
{
    return mediumName();
}

std::string ClientSubsession::codec() const
{
    return codecName();
}

std::string ClientSubsession::toString() const
{
    return name() + '/' + codec();
}

} // namespace client
} // namespace libc2rtsp

