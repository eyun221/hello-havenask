#ifndef FUTURE_LITE_CORO_SEMAPHORE_H
#define FUTURE_LITE_CORO_SEMAPHORE_H

#include <future_lite/Common.h>
#include <future_lite/Executor.h>
#include <future_lite/coro/ConditionVariable.h>
#include <future_lite/coro/SpinLock.h>
#include <future_lite/coro/Lazy.h>
#include <future_lite/experimental/coroutine.h>

namespace future_lite {
namespace coro {

class Semaphore {
public:
    explicit Semaphore(int count = 0) : _count(count) {
    }

    Lazy<> signal() {
        auto lock = co_await _mutex.coScopedLock();
        ++_count;
        _cv.notify();
    }

    Lazy<> wait() {
        auto lock = co_await _mutex.coScopedLock();
        co_await _cv.wait(_mutex, [&] { return _count > 0; });
        --_count;
    }

private:
    SpinLock _mutex;
    ConditionVariable<SpinLock> _cv;
    int _count;
};


}  // namespace coro
}  // namespace future_lite

#endif // FUTURE_LITE_CORO_SEMAPHORE_H
