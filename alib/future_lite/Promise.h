#ifndef FUTURE_LITE_PROMISE_H
#define FUTURE_LITE_PROMISE_H


#include <future_lite/Common.h>
#include <future_lite/Future.h>
#include <exception>

namespace future_lite {

template <typename T>
class Promise {
public:
    Promise()
        : _sharedState(new FutureState<T>())
        , _hasFuture(false)
    {
        _sharedState->attachOne();
    }
    ~Promise() {
        if (_sharedState) {
            _sharedState->detachPromise();
        }
    }

    Promise(const Promise&) = delete;
    Promise& operator=(const Promise&) = delete;

    Promise(Promise<T>&& other)
        : _sharedState(other._sharedState)
        , _hasFuture(other._hasFuture)
    {
        other._sharedState = nullptr;
        other._hasFuture = false;
    }
    Promise& operator=(Promise<T>&& other)
    {
        std::swap(_sharedState, other._sharedState);
        std::swap(_hasFuture, other._hasFuture);
        return *this;
    }

public:
    Future<T> getFuture()
    {
        logicAssert(valid(), "Promise is broken");
        logicAssert(!_hasFuture, "Promise already has a future");
        _hasFuture = true;
        return Future<T>(_sharedState);
    }
    bool valid() const
    {
        return _sharedState != nullptr;
    }

public:
    void setException(std::exception_ptr error) {
        logicAssert(valid(), "Promise is broken");
        _sharedState->setResult(Try<T>(error));
    }
    void setValue(T&& v) {
        logicAssert(valid(), "Promise is broken");
        _sharedState->setResult(Try<T>(std::forward<T>(v)));
    }
    void setValue(Try<T>&& t) {
        logicAssert(valid(), "Promise is broken");
        _sharedState->setResult(std::move(t));
    }

private:

private:
    FutureState<T>* _sharedState;
    bool _hasFuture;
};

}

#endif //FUTURE_LITE_PROMISE_H
