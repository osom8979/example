/**
 * @file   GpuMemory.hpp
 * @brief  GpuMemory class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>

#include <cstdlib>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

// Forward declaration.
class GpuContext;
class GpuStream;
class GpuEvent;
class GpuMemory;
class HostMemory;

/**
 * MemoryWrapper class prototype.
 *
 * @author zer0
 * @date   2018-01-16
 */
class TBAG_API MemoryWrapper : private Noncopyable
{
protected:
    GpuStream const & STREAM;
    std::size_t _capacity;
    std::size_t _size;
    void *      _data;

public:
    MemoryWrapper(GpuStream const & stream) : STREAM(stream), _capacity(0), _size(0), _data(nullptr)
    { /* EMPTY. */ }
    ~MemoryWrapper()
    { /* EMPTY. */ }

public:
    inline GpuStream const & atStream() const TBAG_NOEXCEPT { return STREAM; }
    inline bool isSameStream(GpuStream const & s) const TBAG_NOEXCEPT { return &STREAM == &s; }

    inline std::size_t capacity() const TBAG_NOEXCEPT { return _capacity; }
    inline std::size_t     size() const TBAG_NOEXCEPT { return     _size; }

    inline bool validate() const TBAG_NOEXCEPT { return _data != nullptr && _capacity > 0; }
    inline bool validate(GpuStream const & s) const TBAG_NOEXCEPT { return isSameStream(s) && validate(); }

    inline void       * data()       TBAG_NOEXCEPT { return _data; }
    inline void const * data() const TBAG_NOEXCEPT { return _data; }

    template <typename T> inline T cast() const TBAG_NOEXCEPT { return (T)_data; }

public:
    GpuContext const & atContext() const TBAG_NOEXCEPT;
    bool isSameContext(GpuContext const & c) const TBAG_NOEXCEPT;
};

/**
 * GpuMemory class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuMemory : public MemoryWrapper
{
public:
    GpuMemory(GpuStream const & stream);
    GpuMemory(MemoryWrapper const & mem);
    ~GpuMemory();

public:
    inline void set(void * d, std::size_t c, std::size_t s) TBAG_NOEXCEPT
    { _data = d; _capacity = c; _size = s; }
    inline void clear() TBAG_NOEXCEPT { set(nullptr, 0, 0); }

public:
    Err alloc(std::size_t size);
    Err free();

public:
    Err      copy( GpuMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err      copy(HostMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err copyAsync( GpuMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err copyAsync(HostMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
};

/**
 * HostMemory class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API HostMemory : public MemoryWrapper
{
private:
    HostMemoryFlag _flag;

public:
    HostMemory(GpuStream const & stream);
    HostMemory(MemoryWrapper const & mem);
    ~HostMemory();

public:
    inline void set(void * d, std::size_t c, std::size_t s, HostMemoryFlag f) TBAG_NOEXCEPT
    { _data = d; _capacity = c; _size = s; _flag = f; }
    inline void clear() TBAG_NOEXCEPT { set(nullptr, 0, 0, HostMemoryFlag::HMF_DEFAULT); }

public:
    Err alloc(std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT);
    Err free();

public:
    Err      copy( GpuMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err      copy(HostMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err copyAsync( GpuMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err copyAsync(HostMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
};

using HostMemories = std::vector<HostMemory>;
using  GpuMemories = std::vector<GpuMemory>;

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__

