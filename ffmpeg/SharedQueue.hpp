/**
 * @file   SharedQueue.hpp
 * @brief  SharedQueue class prototype.
 * @author zer0
 * @date   2015-10-25
 */

#ifndef __INCLUDE__SHAREDQUEUE_HPP__
#define __INCLUDE__SHAREDQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <memory>
#include <mutex>
#include <queue>

/**
 * SharedQueue class prototype.
 *
 * @author zer0
 * @date   2015-10-25
 */
template <typename T>
class SharedQueue /* : public Noncopyable */
{
public:
    using Type = T;
    using SharedType = std::shared_ptr<Type>;
    using Queue = std::queue<SharedType>;

    using Mutex = std::mutex;
    using LockGuard = std::lock_guard<Mutex>;

private:
    mutable Mutex _locker;
    Queue _queue;

public:
    SharedQueue() = default;
    virtual ~SharedQueue() = default;

// Queue operations.
public:
    void clear()
    {
        LockGuard guard(_locker);
        while (_queue.empty() == false) {
            _queue.pop();
        }
    }

    bool push(Type * value)
    {
        LockGuard guard(_locker);
        _queue.push(SharedType(value));
        return true;
    }

    bool push(SharedType value)
    {
        LockGuard guard(_locker);
        _queue.push(value);
        return true;
    }

    void pop()
    {
        LockGuard guard(_locker);
        if (_queue.empty()) {
            return;
        }
        _queue.pop();
    }

public:
    SharedType front()
    {
        LockGuard guard(_locker);
        if (_queue.empty()) {
            return SharedType();
        }
        return _queue.front();
    }

    std::size_t size() const
    {
        LockGuard guard(_locker);
        return _queue.size();
    }
};

#endif // __INCLUDE__SHAREDQUEUE_HPP__

