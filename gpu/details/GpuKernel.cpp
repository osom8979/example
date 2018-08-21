/**
 * @file   GpuKernel.cpp
 * @brief  GpuKernel class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuKernel.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

// --------------------------
// GpuProgram implementation.
// --------------------------

GpuProgram::GpuProgram(GpuContext const & context, std::string const & source)
        : GpuIdWrapper(context)
{
    if (isFailure(CONTEXT.createProgram(source, *this))) {
        throw std::bad_alloc();
    }
}

GpuProgram::~GpuProgram()
{
    if (validate()) {
        CONTEXT.releaseProgram(*this);
    }
}

Err GpuProgram::build()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return CONTEXT.buildProgram(*this);
}

// -------------------------
// GpuKernel implementation.
// -------------------------

GpuKernel::GpuKernel(GpuProgram const & program, std::string const & kernel_symbol)
        : GpuIdWrapper(program.CONTEXT), _name(kernel_symbol)
{
    if (isFailure(program.CONTEXT.createKernel(program, kernel_symbol, *this))) {
        throw std::bad_alloc();
    }
}

GpuKernel::~GpuKernel()
{
    CONTEXT.releaseKernel(*this);
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

