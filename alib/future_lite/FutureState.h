#ifndef FUTURE_LITE_FUTURESTATE_H
#define FUTURE_LITE_FUTURESTATE_H

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
#include <cassert>

namespace future_lite {

namespace detail {
    
enum class State : uint8_t {
    START = 0,
    ONLY_RESULT = 1 << 0,
    ONLY_CONTINUATION = 1 << 1,
    DONE = 1 << 5,
};

constexpr State operator|(State lhs, State rhs)
{
    return State((uint8_t)lhs | (uint8_t)rhs);
}
    
constexpr State operator&(State lhs, State rhs)
{
    return State((uint8_t)lhs & (uint8_t)rhs);
}

}

template <typename T>
class FutureState {
private:
    using Continuation = std::function<void(Try<T>&& value)>;

private:
    class ContinuationReference {
    public:
        ContinuationReference() = default;
        explicit ContinuationReference(FutureState<T>* fs)
            : _fs(fs)
        {
            attach();
        }
        ~ContinuationReference() {
            detach();
        }

        ContinuationReference(const ContinuationReference& other)
            : _fs(other._fs)
        {
            attach();
        }
        ContinuationReference& operator=(const ContinuationReference& other) = delete;

        ContinuationReference(ContinuationReference&& other)
            : _fs(other._fs)
        {
            other._fs = nullptr;
        }

        ContinuationReference& operator=(ContinuationReference&& other) = delete;

        FutureState* getFutureState() const noexcept
        {
            return _fs;
        }
    private:
        void attach() {
            if (_fs) {
                _fs->attachOne();
                _fs->refContinuation();
            }
        }
        void detach()
        {
            if (_fs) {
                _fs->derefContinuation();
                _fs->detachOne();
            }
        }

    private:
        FutureState<T>* _fs = nullptr;
    };

public:
    FutureState()
        : _state(detail::State::START)
        , _attached(0)
        , _continuationRef(0)
        , _executor(nullptr)
    {
    }
    ~FutureState() {}

    FutureState(const FutureState&) = delete;
    FutureState& operator=(const FutureState&) = delete;

    FutureState(FutureState&& other) = delete;
    FutureState& operator=(FutureState&&) = delete;

public:
    bool hasResult() const noexcept
    {
        constexpr auto allow = detail::State::DONE | detail::State::ONLY_RESULT;
        auto state = _state.load(std::memory_order_acquire);
        return (state & allow) != detail::State();
    }

    bool hasContinuation() const noexcept
    {
        constexpr auto allow = detail::State::DONE | detail::State::ONLY_CONTINUATION;
        auto state = _state.load(std::memory_order_acquire);
        return (state & allow) != detail::State();
    }

    inline __attribute__((__always_inline__)) void attachOne()
    {
        _attached.fetch_add(1, std::memory_order_relaxed);
    }
    inline __attribute__((__always_inline__)) void detachOne()
    {
        auto old = _attached.fetch_sub(1, std::memory_order_relaxed);
        assert(old >= 1u);
        if (old == 1) {
            delete this;
        }
    }
    inline __attribute__((__always_inline__)) void detachPromise()
    {
        if (!hasResult()) {
            try {
                throw std::runtime_error("Promise is broken");
            } catch (...) {
                setResult(Try<T>(std::current_exception()));
            }
        }
        detachOne();
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

public:
    // State Transfer

    void setResult(Try<T>&& value)
    {
        logicAssert(!hasResult(), "FutureState already has a result");
        _try = std::move(value);

        auto state = _state.load(std::memory_order_acquire);
        switch (state) {
        case detail::State::START:
            if (_state.compare_exchange_strong(state, detail::State::ONLY_RESULT,
                                               std::memory_order_release)) {
                return;
            }
            // state has already transfered, fallthrough
            assert(_state.load(std::memory_order_relaxed) == detail::State::ONLY_CONTINUATION);
        case detail::State::ONLY_CONTINUATION:
            if (_state.compare_exchange_strong(state, detail::State::DONE,
                                               std::memory_order_release)) {
                scheduleContinuation(false);
                return;
            }
        default:
            logicAssert(false, "State Transfer Error");
        }
    }

    template <typename F>
    void setContinuation(F&& func)
    {
        logicAssert(!hasContinuation(), "FutureState already has a continuation");
        MoveWrapper<F> lambdaFunc (std::move(func));
        new (&_continuation) Continuation([lambdaFunc](Try<T>&& v) mutable {
            auto& lambda = lambdaFunc.get();
            lambda(std::forward<Try<T>>(v));
        });

        auto state
            = _state.load(std::memory_order_acquire);
        switch (state) {
        case detail::State::START:
            if (_state.compare_exchange_strong(state, detail::State::ONLY_CONTINUATION,
                                               std::memory_order_release)) {
                return;
            }
            // state has already transfered, fallthrough
            assert(_state.load(std::memory_order_relaxed) == detail::State::ONLY_RESULT);
        case detail::State::ONLY_RESULT:
            if (_state.compare_exchange_strong(state, detail::State::DONE,
                                               std::memory_order_release)) {
                scheduleContinuation(true);
                return;
            }
        default:
            logicAssert(false, "State Transfer Error");
        }
    }

    bool currentThreadInExecutor() const {
        if (!_executor) {
            return false;
        }
        return _executor->currentThreadInExecutor();
    }
private:
    void scheduleContinuation(bool triggerByContinuation)
    {
        logicAssert(_state.load(std::memory_order_relaxed) == detail::State::DONE,
            "FutureState is not DONE");
        if (!_executor || triggerByContinuation || currentThreadInExecutor()) {
            ContinuationReference guard(this);
            _continuation(std::move(_try));
        } else {
            ContinuationReference guard(this);
            ContinuationReference guardForException(this);
            try {
                bool ret = _executor->schedule([fsRef = std::move(guard)]() mutable {
                    auto ref = std::move(fsRef);
                    auto fs = ref.getFutureState();
                    fs->_continuation(std::move(fs->_try));
                });
                if (!ret) {
                    throw std::runtime_error("schedule continuation in executor failed");                    
                }
            } catch (std::exception& e) {
                FL_LOG(WARN, "%s", e.what());
                _continuation(std::move(_try));
            }
        }
    }

    void refContinuation()
    {
        _continuationRef.fetch_add(1, std::memory_order_relaxed);
    }
    void derefContinuation()
    {
        auto old = _continuationRef.fetch_sub(1, std::memory_order_relaxed);
        assert(old >= 1);
        if (old == 1) {
            _continuation.~Continuation();
        }
    }

private:
    std::atomic<detail::State> _state;
    std::atomic<uint8_t> _attached;
    std::atomic<uint8_t> _continuationRef;
    Try<T> _try;
    union {
        Continuation _continuation;
    };
    Executor* _executor;

private:
    FL_LOG_DECLARE();
};

//////////////////////////
FL_LOG_SETUP_TEMPLATE(future_lite, FutureState, T);

}

#endif
