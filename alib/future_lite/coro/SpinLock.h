#ifndef FUTURE_LITE_CORO_SPIN_LOCK_H
#define FUTURE_LITE_CORO_SPIN_LOCK_H

#include <future_lite/coro/Lazy.h>

namespace future_lite {
namespace coro {


class SpinLock {
public:
    explicit SpinLock(std::int32_t count = 1024) noexcept
        : _spinCount(count), _locked(false) {
    }

    bool tryLock() noexcept {
        return !_locked.exchange(true, std::memory_order_acquire);
    }

    Lazy<> lock() noexcept {
        auto counter = _spinCount;
        while (!tryLock()) {
            while (_locked.load(std::memory_order_relaxed)) {
                if (counter-- <= 0) {
                    co_await Yield{};
                    counter = _spinCount;
                }
            }
        }
        co_return;
    }

    void unlock() noexcept {
        _locked.store(false, std::memory_order_release);
    }

    Lazy<std::unique_lock<SpinLock>> coScopedLock() noexcept {
        co_await lock();
        co_return std::unique_lock<SpinLock>{*this, std::adopt_lock};
    }

private:
    std::int32_t _spinCount;
    std::atomic<bool> _locked;
};

} // namespace core
} // namespace future_lite

#endif // FUTURE_LITE_CORO_SPIN_LOCK_H
