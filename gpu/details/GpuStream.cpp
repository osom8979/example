/**
 * @file   GpuStream.cpp
 * @brief  GpuStream class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuStream.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/log/Log.hpp>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

GpuStream::GpuStream(GpuContext const & context) : GpuIdWrapper(context)
{
    if (isFailure(context.createStream(*this))) {
        throw std::bad_alloc();
    }
}

GpuStream::~GpuStream()
{
    if (validate()) {
        CONTEXT.releaseStream(*this);
    }
}

Err GpuStream::flush() const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return CONTEXT.flush(*this);
}

Err GpuStream::finish() const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return CONTEXT.finish(*this);
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

