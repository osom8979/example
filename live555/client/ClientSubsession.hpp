/**
 * @file   ClientSubsession.hpp
 * @brief  ClientSubsession class prototype.
 * @author zer0
 * @date   2018-10-02
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_CLIENTSUBSESSION_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_CLIENTSUBSESSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libc2rtsp/export.h>
#include <liveMedia.hh>

#include <string>

namespace libc2rtsp {
namespace client    {

// Forward declarations.
class ClientSession;

/**
 * ClientSubsession class prototype.
 *
 * @author zer0
 * @date   2018-10-02
 */
class C2RTSP_API ClientSubsession : public MediaSubsession
{
public:
    friend class ClientSession;

protected:
    ClientSubsession(ClientSession & parent);
    virtual ~ClientSubsession();

public:
    std::string name() const;
    std::string codec() const;

public:
    std::string toString() const;
};

} // namespace client
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_CLIENTSUBSESSION_HPP__

