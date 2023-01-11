#ifndef FUTURE_LITE_LOCALSTATE_H
#define FUTURE_LITE_LOCALSTATE_H

#include <atomic>

#include <future_lite/Common.h>
#include <future_lite/Executor.h>
#include <future_lite/Try.h>
#include <functional>
#include <stdexcept>
#include <thread>
#include <future_lite/MoveWrapper.h>
#include <condition_variable>
#include <mutex>
#include <utility>

namespace future_lite {

template <typename T>
class LocalState {
private:
    using Continuation = std::function<void(Try<T>&& value)>;

public:
    LocalState()
        : _executor(nullptr)
    {
    }
    LocalState(T&& v)
        : _try(std::forward<T>(v))
        , _executor(nullptr)
    {}
    LocalState(Try<T>&& t)
        : _try(std::move(t))
        , _executor(nullptr)
    {}

    ~LocalState() {}

    LocalState(const LocalState&) = delete;
    LocalState& operator=(const LocalState&) = delete;

    LocalState(LocalState&& other)
        : _try(std::move(other._try))
        , _executor(std::exchange(other._executor, nullptr))
    {
    }
    LocalState& operator=(LocalState&& other)
    {
        if (this != &other) {
            std::swap(_try, other._try);
            std::swap(_executor, other._executor);
        }
        return *this;
    }

public:
    bool hasResult() const noexcept
    {
        return _try.available();
    }

public:
    Try<T>& getTry() noexcept
    {
        if (!hasResult()) {
            FL_LOG(ERROR, "Try is not available yet!");
        }
        return _try;
    }
    const Try<T>& getTry() const noexcept
    {
        if (!hasResult()) {
            FL_LOG(ERROR, "Try is not available yet!");
        }
        return _try;
    }

    void setExecutor(Executor* ex)
    {
        _executor = ex;
    }

    Executor* getExecutor() { return _executor; }

    bool currentThreadInExecutor() const {
        if (!_executor) {
            return false;
        }
        return _executor->currentThreadInExecutor();
    }

    template <typename F>
    void setContinuation(F&& f)
    {
        assert(_try.available());
        std::forward<F>(f)(std::move(_try));
    }

private:
    Try<T> _try;
    Executor* _executor;

private:
    FL_LOG_DECLARE();
};

//////////////////////////
FL_LOG_SETUP_TEMPLATE(future_lite, LocalState, T);

}

#endif
