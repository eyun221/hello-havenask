#ifndef FUTURE_LITE_EXECUTOR_H
#define FUTURE_LITE_EXECUTOR_H


#include <functional>
#include <future_lite/Config.h>
#include <future_lite/IOExecutor.h>
#include <future_lite/util/Condition.h>
#include <string>
#include <chrono>
#include <thread>


#if FUTURE_LITE_USE_COROUTINES
#include <future_lite/experimental/coroutine.h>
#endif

namespace future_lite {

struct ExecutorStat {
    size_t pendingTaskCount = 0;
    ExecutorStat() = default;
};

struct CurrentExecutor {};


class Executor {
public:
    using Context = void *;
    static constexpr Context NULLCTX = nullptr;
    using Duration = std::chrono::duration<int64_t, std::micro>;

    using Func = std::function<void()>;
    class Awaitable;
    class Awaiter;
    class TimeAwaitable;
    class TimeAwaiter;

    Executor(std::string name = "default") : _name(std::move(name)){};
    virtual ~Executor() {};

    Executor(const Executor &) = delete;
    Executor& operator = (const Executor &) = delete;
public:
    virtual bool schedule(Func func) = 0;
    virtual bool currentThreadInExecutor() const = 0;
    virtual ExecutorStat stat() const = 0;

    virtual size_t currentContextId() const { return 0; };

    const std::string &name() const { return _name; }


    bool syncSchedule(Func func) {
        util::Condition cond;
        auto ret = schedule([f = std::move(func), &cond]() {
            f();
            cond.set();
        });
        if (!ret) {
            return false;
        }
        cond.wait();
        return true;
    }

    virtual IOExecutor* getIOExecutor() = 0;
    virtual Context checkout() { return NULLCTX; }
    virtual bool checkin(Func func, Context ctx) { return schedule(std::move(func)); }        

#if FUTURE_LITE_USE_COROUTINES
    Awaitable schedule();
    TimeAwaitable after(Duration dur);
#endif

private:

    virtual void schedule(Func func, Duration dur) {
        std::thread([this, func = std::move(func), dur]() {
            std::this_thread::sleep_for(dur);
            schedule(std::move(func));
        }).detach();
    }

private:
    std::string _name;
};

#if FUTURE_LITE_USE_COROUTINES

class Executor::Awaiter {
public:
    Awaiter(Executor *ex) : _ex(ex) {}

public:
    bool await_ready() const noexcept {
        if (_ex->currentThreadInExecutor()) {
            return true;
        }
        return false;
    }

    template <typename PromiseType>
    void await_suspend(
        std::experimental::coroutine_handle<PromiseType> continuation)
    {
        std::function<void()> func = [c = continuation]() mutable { c.resume(); };
        _ex->schedule(func);
    }
    void await_resume() const noexcept {}

private:
    Executor *_ex;
};

class Executor::Awaitable {
public:
    Awaitable(Executor *ex) : _ex(ex) {}

    auto coAwait(Executor *) { return Executor::Awaiter(_ex); }

private:
    Executor *_ex;
};

Executor::Awaitable
inline Executor::schedule() {
    return Executor::Awaitable(this);
};

/////// TimeAwait

class Executor::TimeAwaiter {
public:
    TimeAwaiter(Executor *ex, Executor::Duration dur) : _ex(ex), _dur(dur) {}

public:
    bool await_ready() const noexcept {
        return false;
    }

    template <typename PromiseType>
    void await_suspend(
        std::experimental::coroutine_handle<PromiseType> continuation)
    {
        std::function<void()> func = [c = continuation]() mutable {
            c.resume();
        };
        _ex->schedule(func, _dur);
    }
    void await_resume() const noexcept {}

private:
    Executor *_ex;
    Executor::Duration _dur;
};

class Executor::TimeAwaitable {
public:
    TimeAwaitable(Executor *ex, Executor::Duration dur) : _ex(ex), _dur(dur) {}

    auto coAwait(Executor *) { return Executor::TimeAwaiter(_ex, _dur); }

private:
    Executor *_ex;
    Executor::Duration _dur;
};

Executor::TimeAwaitable
inline Executor::after(Executor::Duration dur) {
    return Executor::TimeAwaitable(this, dur);
};

#endif

}  // namespace future_lite

#endif
