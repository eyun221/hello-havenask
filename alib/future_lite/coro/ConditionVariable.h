#ifndef FUTURE_LITE_CORO_CONDITION_VARIABLE_H
#define FUTURE_LITE_CORO_CONDITION_VARIABLE_H

#include <future_lite/Common.h>
#include <future_lite/Executor.h>
#include <future_lite/coro/Lazy.h>
#include <future_lite/coro/CoAwait.h>

namespace future_lite {
namespace coro {

template<class Lock>
class ConditionVariableAwaiter;

template<class Lock = void>
class ConditionVariable {
public:
    ConditionVariable() noexcept {}
    ~ConditionVariable() {}

    ConditionVariable(const ConditionVariable&) = delete;
    ConditionVariable& operator=(const ConditionVariable&) = delete;

    void notify() noexcept;

    template<class Pred>
    Lazy<> wait(Lock& lock, Pred&& pred) noexcept;

private:
    void resumeWaiters(ConditionVariableAwaiter<Lock>* awaiters);

private:
    friend class ConditionVariableAwaiter<Lock>;
    Executor* _executor = nullptr;
    std::atomic<ConditionVariableAwaiter<Lock> *> _awaiters = nullptr;

};

template<class Lock = void>
class ConditionVariableAwaiter {
public:
    ConditionVariableAwaiter(ConditionVariable<Lock>* cv, Lock& lock) noexcept
        : _cv(cv), _lock(lock) {}

    bool await_ready() const noexcept {
        return false;
    }

    bool await_suspend(std::experimental::coroutine_handle<>
                       continuation) noexcept {
        _continuation = continuation;
        std::unique_lock<Lock> lock{_lock, std::adopt_lock};
        auto awaitings = _cv->_awaiters.load(std::memory_order_relaxed);
        do {
            _next = awaitings;
        } while (!_cv->_awaiters.compare_exchange_weak(
                     awaitings,
                     this,
                     std::memory_order_acquire,
                     std::memory_order_relaxed));
        return true;
    }
    void await_resume() const noexcept {}

public:
    ConditionVariable<Lock>* _cv;
    Lock& _lock;

private:
    friend class ConditionVariable<Lock>;
    ConditionVariableAwaiter<Lock>* _next = nullptr;
    std::experimental::coroutine_handle<> _continuation;
};

template<class Lock>
template<class Pred>
inline Lazy<> ConditionVariable<Lock>::wait(Lock& lock, Pred&& pred) noexcept {
    while (!pred()) {
        co_await ConditionVariableAwaiter<Lock>{this, lock};
        co_await lock.lock();
    }
    co_return;
}

template<class Lock>
inline void ConditionVariable<Lock>::notify() noexcept {
    auto awaitings = _awaiters.load(std::memory_order_relaxed);
    while (!_awaiters.compare_exchange_weak(
               awaitings,
               nullptr,
               std::memory_order_release,
               std::memory_order_relaxed));
    resumeWaiters(awaitings);
}

template<class Lock>
inline void ConditionVariable<Lock>::resumeWaiters(
    ConditionVariableAwaiter<Lock>* awaiters) {
    for (auto head = awaiters; head; head = head->_next) {
        head->_continuation.resume();
    }
}

template<>
class ConditionVariableAwaiter<>;
template<>
class ConditionVariable<> {
public:
    using pointer_type = void *;

public:
    ConditionVariable() noexcept {}
    ~ConditionVariable() {}

    ConditionVariable(const ConditionVariable&) = delete;
    ConditionVariable& operator=(const ConditionVariable&) = delete;

    void notify() noexcept;
    ConditionVariableAwaiter<> wait() noexcept;
    void reset() noexcept;

private:
    void resumeWaiters(ConditionVariableAwaiter<>* awaiters);

private:
    friend class ConditionVariableAwaiter<>;
    std::atomic<pointer_type> _awaiters = nullptr;
};

template<>
class ConditionVariableAwaiter<> {
public:
    using pointer_type = void *;

public:
    ConditionVariableAwaiter(ConditionVariable<>* cv) noexcept
        : _cv(cv) {}

    bool await_ready() const noexcept {
        return static_cast<pointer_type>(_cv) ==
            _cv->_awaiters.load(std::memory_order_acquire);
    }
    bool await_suspend(std::experimental::coroutine_handle<>
                       continuation) noexcept {
        _continuation = continuation;
        pointer_type awaitings = _cv->_awaiters.load(std::memory_order_acquire);
        do {
            if (awaitings == static_cast<pointer_type>(_cv)) {
                return false;
            }
            _next = static_cast<ConditionVariableAwaiter<>*>(awaitings);
        } while (!_cv->_awaiters.compare_exchange_weak(
                     awaitings, static_cast<pointer_type>(this),
                     std::memory_order_release,
                     std::memory_order_acquire));
        return true;
    }
    void await_resume() const noexcept {}

public:
    ConditionVariable<>* _cv;

private:
    friend class ConditionVariable<>;
    ConditionVariableAwaiter<>* _next = nullptr;
    std::experimental::coroutine_handle<> _continuation;
};

inline ConditionVariableAwaiter<> ConditionVariable<>::wait() noexcept {
    return ConditionVariableAwaiter<>{this};
}

inline void ConditionVariable<>::notify() noexcept {
    pointer_type self = static_cast<pointer_type>(this);
    pointer_type awaitings = _awaiters.exchange(self, std::memory_order_acq_rel);
    if (awaitings != self) {
        resumeWaiters(static_cast<ConditionVariableAwaiter<>*>(awaitings));
    }
}

inline void ConditionVariable<>::reset() noexcept {
    pointer_type self = static_cast<pointer_type>(this);
    _awaiters.compare_exchange_strong(self, nullptr, std::memory_order_relaxed);
}

inline void ConditionVariable<>::resumeWaiters(ConditionVariableAwaiter<>* awaiters) {
    for (auto head = awaiters; head; head = head->_next) {
        head->_continuation.resume();
    }
}

using Notifier = ConditionVariable<>;


} // namespace coro
} // namespace future_lite


#endif // FUTURE_LITE_CORO_CONDITION_VARIABLE_H
