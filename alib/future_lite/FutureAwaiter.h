#ifndef FUTURE_LITE_FUTURE_AWAITER_H
#define FUTURE_LITE_FUTURE_AWAITER_H

#include <future_lite/Coroutines.h>
#include <type_traits>
#include <unistd.h>

namespace future_lite {

template <typename FutureType>
struct FutureAwaiter {
    using valueType = typename FutureType::value_type;
    FutureAwaiter(FutureType&& f) : _f(std::move(f)) {}
    FutureAwaiter(FutureAwaiter&) = delete;
    FutureAwaiter(FutureAwaiter&& rhs) : _f(std::move(rhs._f)) {}
#if FUTURE_LITE_USE_COROUTINES
    bool await_ready() {
        if (_f.hasResult()) {
            return true;
        } else {
            return false;
        }
    }
    void await_suspend(std::experimental::coroutine_handle<> h) {
        auto func = [&](Try<valueType>&& t) {
            h.resume();
        };
        std::move(_f).thenTry(func);
    }
    valueType await_resume() { return std::move(_f.value()); }
    FutureAwaiter& coAwait(Executor*) { return *this; }
#else
    operator valueType()&& { return std::move(_f).get(); }
#endif
    FutureType _f;
};

}  // namespace future_lite

#endif  //FUTURE_LITE_FUTURE_AWAITER_H
