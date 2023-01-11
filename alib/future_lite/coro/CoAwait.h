#ifndef FUTURE_LITE_CORO_CO_AWAIT_H
#define FUTURE_LITE_CORO_CO_AWAIT_H


#include <future_lite/Common.h>
#include <future_lite/Invoke.h>
#include <future_lite/Executor.h>
#include <future_lite/coro/Traits.h>
#include <exception>

#include <mutex>
#include <atomic>
#include <utility>

namespace future_lite {
namespace coro {

namespace detail {

class ViaCoroutine {
public:
    struct promise_type {
        struct FinalAwaiter;
        promise_type(Executor* ex) : _ex(ex), _ctx(Executor::NULLCTX) {}
        ViaCoroutine get_return_object() noexcept;
        void return_void() noexcept {}
        void unhandled_exception() const noexcept { assert(false); }

        std::experimental::suspend_always initial_suspend() const noexcept { return {}; }
        FinalAwaiter final_suspend() { return FinalAwaiter(_ctx); }

        struct FinalAwaiter {
            FinalAwaiter(Executor::Context ctx) : _ctx(ctx) {}
            bool await_ready() const noexcept { return false; }

            template<typename PromiseType>
            auto await_suspend(std::experimental::coroutine_handle<PromiseType> h) {
                auto& pr = h.promise();
                // promise will remain valid across final_suspend point
                if (pr._ex) {
                    std::function<void()> func = [&pr]() {
                        pr._continuation.resume();
                    };
                    pr._ex->checkin(func, _ctx);
                } else {
                    pr._continuation.resume();
                }
            }
            void await_resume() const noexcept {}

            Executor::Context _ctx;
        };

        Executor* _ex;
        std::experimental::coroutine_handle<> _continuation;
        Executor::Context _ctx;
    };

    ViaCoroutine(std::experimental::coroutine_handle<promise_type> coro) : _coro(coro) {}
    ~ViaCoroutine() {
        if (_coro) {
            _coro.destroy();
            _coro = nullptr;
        }
    }

    ViaCoroutine(const ViaCoroutine&) = delete;
    ViaCoroutine& operator=(const ViaCoroutine&) = delete;
    ViaCoroutine(ViaCoroutine&& other) : _coro(std::exchange(other._coro, nullptr)) {}
public:
    static ViaCoroutine create(Executor* ex) { co_return; }

public:
    std::experimental::coroutine_handle<> getWrappedContinuation(
        std::experimental::coroutine_handle<> continuation) {
        // do not call this method on a moved ViaCoroutine,
        assert(_coro);
        auto& pr = _coro.promise();
        if (pr._ex) {
            pr._ctx = pr._ex->checkout();
        }
        pr._continuation = continuation;
        return _coro;
    }

private:
    std::experimental::coroutine_handle<promise_type> _coro;
};

inline ViaCoroutine ViaCoroutine::promise_type::get_return_object() noexcept {
    return ViaCoroutine(std::experimental::coroutine_handle<ViaCoroutine::promise_type>::from_promise(*this));
}


// used by co_await non-Lazy object
template <typename Awaiter>
struct [[nodiscard]] ViaAsyncAwaiter {
    template<typename Awaitable>
    ViaAsyncAwaiter(Executor * ex, Awaitable && awaitable)
        : _ex(ex),
        _awaiter(detail::getAwaiter(std::forward<Awaitable>(awaitable))),
        _viaCoroutine(ViaCoroutine::create(ex)) {}

    bool await_ready() { return _awaiter.await_ready(); }

    auto await_suspend(std::experimental::coroutine_handle<> continuation) {
        return _awaiter.await_suspend(_viaCoroutine.getWrappedContinuation(continuation));
    }

    auto await_resume() { return _awaiter.await_resume(); }

    Executor* _ex;
    Awaiter _awaiter;
    ViaCoroutine _viaCoroutine;
};

template <typename Awaitable,
          std::enable_if_t<!detail::HasCoAwaitMethod<std::decay_t<Awaitable>>::value, int> = 0>
inline auto coAwait(Executor* ex, Awaitable&& awaitable) {
    using AwaiterType = std::decay_t<decltype(detail::getAwaiter(std::forward<Awaitable>(awaitable)))>;
    return ViaAsyncAwaiter<AwaiterType>(ex, std::forward<Awaitable>(awaitable));
}

template <typename Awaitable,
          std::enable_if_t<detail::HasCoAwaitMethod<std::decay_t<Awaitable>>::value, int> = 0>
inline auto coAwait(Executor* ex, Awaitable&& awaitable) {
    return std::forward<Awaitable>(awaitable).coAwait(ex);
}

}  // namespace detail

}  // namespace coro
}  // namespace future_lite

#endif
