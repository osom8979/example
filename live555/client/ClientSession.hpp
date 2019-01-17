/**
 * @file   ClientSession.hpp
 * @brief  ClientSession class prototype.
 * @author zer0
 * @date   2018-10-02
 */

#ifndef __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_CLIENTSESSION_HPP__
#define __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_CLIENTSESSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/predef.hpp>
#include <libc2rtsp/export.h>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

#include <string>

namespace libc2rtsp {
namespace client    {

/**
 * ClientSession class prototype.
 *
 * @author zer0
 * @date   2018-10-02
 */
class C2RTSP_API ClientSession : public MediaSession
{
protected:
    ClientSession(UsageEnvironment & env);
    virtual ~ClientSession();

public:
    static ClientSession * createNew(UsageEnvironment & env, std::string const & sdp_description);

protected:
    virtual MediaSubsession * createNewMediaSubsession() override;
};

} // namespace client
} // namespace libc2rtsp

#endif // __INCLUDE_LIBC2CORE__LIBC2RTSP_CLIENT_CLIENTSESSION_HPP__

