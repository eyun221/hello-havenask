#ifndef FUTURE_LITE_COROUTINES_H
#define FUTURE_LITE_COROUTINES_H

#include <future_lite/Config.h>
#include <vector>

#if FUTURE_LITE_USE_COROUTINES

namespace future_lite {

static constexpr bool USE_COROUTINES = true;

}

#define FL_COAWAIT co_await
#define FL_CORETURN co_return

#include <future_lite/coro/Lazy.h>
#include <future_lite/coro/Task.h>
#include <future_lite/coro/Generator.h>
#include <future_lite/coro/AsyncGenerator.h>
#include <future_lite/coro/Trace.h>

#define FL_LAZY(...) future_lite::coro::Lazy<__VA_ARGS__>
constexpr CurrentExecutor GetCurrentExecutor() { return {}; }


#else // Fake Coroutines

namespace future_lite {

static constexpr bool USE_COROUTINES = false;

}

#define FL_COAWAIT
#define FL_CORETURN return
#define FL_LAZY(...) __VA_ARGS__

#define FL_TRACE(...)
#define FL_CORO_TRACE(...)

//
namespace future_lite{

class Executor;

constexpr Executor* GetCurrentExecutor() { return nullptr; }

template <typename T>
inline T getTryValue(T&& v) {
    return std::forward<T>(v);
}

inline void getTryValue() {}

namespace coro{

template <typename T>
inline T syncAwait(T&& v, Executor* ex = nullptr) {
    // schedule on another thread in executor?
    return std::forward<T>(v);
}

inline void syncAwait(...) {
    return;
}

template <typename T>
inline std::vector<T> collectAll(std::vector<T>&& vec) {
    return std::move(vec);
}

template <typename T, typename InAlloc, typename OutAlloc>
inline std::vector<T, InAlloc> collectAll(std::vector<T, InAlloc>&& vec, OutAlloc alloc) {
    return std::move(vec);
}

}  // namespace coro
}  // namespace future_lite

#endif

#endif
