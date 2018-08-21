/**
 * @file   GpuEvent.cpp
 * @brief  GpuEvent class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuEvent.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/gpu/details/GpuStream.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

// ------------------------
// GpuEvent implementation.
// ------------------------

GpuEvent::GpuEvent(GpuStream const & stream)
        : STREAM(stream), _start(UNKNOWN_ID), _stop(UNKNOWN_ID)
{
    if (isFailure(STREAM.atContext().createEvent(stream, *this))) {
        throw std::bad_alloc();
    }
}

GpuEvent::~GpuEvent()
{
    if (validate()) {
        STREAM.atContext().releaseEvent(*this);
    }
}

GpuContext const & GpuEvent::atContext() const TBAG_NOEXCEPT
{
    return STREAM.atContext();
}

bool GpuEvent::isSameContext(GpuContext const & c) const TBAG_NOEXCEPT
{
    return STREAM.isSameContext(c);
}

Err GpuEvent::sync()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return STREAM.atContext().syncEvent(*this);
}

Err GpuEvent::elapsed(float * millisec)
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return STREAM.atContext().elapsedEvent(*this, millisec);
}

float GpuEvent::elapsed()
{
    float millisec = 0;
    if (isSuccess(elapsed(&millisec))) {
        return millisec;
    }
    return 0.0f;
}

// -----------------------------
// CpuEventGuard implementation.
// -----------------------------

CpuEventGuard::CpuEventGuard(GpuEvent * e) : _event(e)
{
    if (_event != nullptr) {
        _event->setStart(nowNano());
    }
}

CpuEventGuard::~CpuEventGuard()
{
    if (_event != nullptr) {
        _event->setStop(nowNano());
    }
}

GpuId CpuEventGuard::nowNano()
{
    using namespace std::chrono;
    return (GpuId)duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

