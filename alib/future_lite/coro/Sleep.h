#ifndef FUTURE_LITE_CORO_SLEEP_H
#define FUTURE_LITE_CORO_SLEEP_H

#include "future_lite/coro/Lazy.h"
#include "future_lite/Executor.h"
#include <thread>

namespace future_lite {
namespace coro {

template <typename Rep, typename Period>
Lazy<void> sleep(std::chrono::duration<Rep, Period> dur) {
    auto ex = co_await CurrentExecutor();
    if (!ex) {
        std::this_thread::sleep_for(dur);
        co_return;
    }
    co_return co_await ex->after(std::chrono::duration_cast<Executor::Duration>(dur));
}
}
}  // namespace future_lite

#endif
