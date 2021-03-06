/**
 * @file   GpuEvent.hpp
 * @brief  GpuEvent class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUEVENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUEVENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

// Forward declaration.
class GpuContext;
class GpuStream;

/**
 * GpuEvent class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuEvent : private Noncopyable
{
private:
    GpuStream const & STREAM;
    GpuId _start;
    GpuId _stop;

public:
    GpuEvent(GpuStream const & s);
    ~GpuEvent();

public:
    inline GpuStream const & atStream() const TBAG_NOEXCEPT { return STREAM; }
    inline bool isSameStream(GpuStream const & s) const TBAG_NOEXCEPT { return &STREAM == &s; }

    inline bool validate() const TBAG_NOEXCEPT { return _start != UNKNOWN_ID; }
    inline bool validate(GpuStream const & s) const TBAG_NOEXCEPT { return isSameStream(s) && validate(); }

    inline GpuId        getId() const TBAG_NOEXCEPT { return _start; }
    inline GpuId       & atId()       TBAG_NOEXCEPT { return _start; }
    inline GpuId const & atId() const TBAG_NOEXCEPT { return _start; }

    template <typename T> inline void    setId(T i) TBAG_NOEXCEPT { _start = (GpuId)i; }
    template <typename T> inline void setStart(T i) TBAG_NOEXCEPT { _start = (GpuId)i; }
    template <typename T> inline void  setStop(T i) TBAG_NOEXCEPT {  _stop = (GpuId)i; }

    inline GpuId getStart() const TBAG_NOEXCEPT { return _start; }
    inline GpuId  getStop() const TBAG_NOEXCEPT { return  _stop; }

    template <typename T> inline T    castId() const TBAG_NOEXCEPT { return (T)_start; }
    template <typename T> inline T castStart() const TBAG_NOEXCEPT { return (T)_start; }
    template <typename T> inline T  castStop() const TBAG_NOEXCEPT { return (T) _stop; }

    inline void clearIds() TBAG_NOEXCEPT { _start = UNKNOWN_ID; _stop = UNKNOWN_ID; }

public:
    GpuContext const & atContext() const TBAG_NOEXCEPT;
    bool isSameContext(GpuContext const & c) const TBAG_NOEXCEPT;

public:
    Err sync();
    Err elapsed(float * millisec);
    float elapsed();
};

using SharedGpuEvent = std::shared_ptr<GpuEvent>;
using WeakedGpuEvent = std::weak_ptr<GpuEvent>;

/**
 * CpuEventGuard class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API CpuEventGuard : private Noncopyable
{
private:
    GpuEvent * _event;

public:
    CpuEventGuard(GpuEvent * e = nullptr);
    ~CpuEventGuard();

public:
    static GpuId nowNano();
};

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUEVENT_HPP__

