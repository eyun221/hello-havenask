#ifndef FUTURE_LITE_CORO_UTIL_H
#define FUTURE_LITE_CORO_UTIL_H


#include <future_lite/Common.h>
#include <future_lite/Invoke.h>
#include <future_lite/experimental/coroutine.h>
#include <exception>

#include <mutex>
#include <atomic>

namespace future_lite {

namespace coro {

namespace detail {

struct DetachedCoroutine {
    struct promise_type {
        std::experimental::suspend_never initial_suspend() noexcept { return {}; }
        std::experimental::suspend_never final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::rethrow_exception(std::current_exception()); }
        DetachedCoroutine get_return_object() noexcept { return DetachedCoroutine(); }
    };
};

}  // namespace detail

template <typename T>
struct ReadyAwaiter {
    ReadyAwaiter(T value) : _value(std::move(value)) {}

    bool await_ready() const noexcept { return true; }
    void await_suspend(CoroHandle<>) const noexcept {}
    T await_resume() noexcept { return std::move(_value); }

    T _value;
};

}  // namespace coro
}  // namespace future_lite



#endif
