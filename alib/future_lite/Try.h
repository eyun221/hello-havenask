#ifndef FUTURE_LITE_TRY_H
#define FUTURE_LITE_TRY_H


#include <future_lite/Common.h>
#include <future_lite/Invoke.h>
#include <future_lite/Unit.h>
#include <exception>
#include <cassert>

namespace future_lite {

template <typename T>
class Try {
public:
    using value_type = T;

private:
    enum class Contains {
        VALUE,
        EXCEPTION,
        NOTHING,
    };

public:
    Try()
        : _contains(Contains::NOTHING)
    {
    }
    ~Try()
    {
        destroy();
    }

    Try(Try<T>&& other)
        : _contains(other._contains)
    {
        if (_contains == Contains::VALUE) {
            new (&_value) T(std::move(other._value));
        } else if (_contains == Contains::EXCEPTION) {
            new (&_error) std::exception_ptr(other._error);
        }
    }
    template<typename T2 = T>
    Try(std::enable_if_t<std::is_same<Unit, T2>::value, const Try<void>&> other)
    {
        if (other.hasError()) {
            _contains = Contains::EXCEPTION;
            new (&_error) std::exception_ptr(other._error);
        } else {
            _contains = Contains::VALUE;
            new (&_value) T();
        }
    }
    Try& operator=(Try<T>&& other)
    {
        if (&other == this) {
            return *this;
        }
        
        destroy();

        _contains = other._contains;
        if (_contains == Contains::VALUE) {
            new (&_value) T(std::move(other._value));
        } else if (_contains == Contains::EXCEPTION) {
            new (&_error) std::exception_ptr(other._error);
        }
        return *this;
    }
    Try& operator=(std::exception_ptr error)
    {
        if (_contains == Contains::EXCEPTION && error == this->_error) {
            return *this;
        }

        destroy();

        _contains = Contains::EXCEPTION;
        new (&_error) std::exception_ptr(error);
        return *this;
    }

    Try(const T& val)
        : _contains(Contains::VALUE)
        , _value(val)
    {
    }
    Try(T&& val)
        : _contains(Contains::VALUE)
        , _value(std::move(val))
    {
    }
    Try(std::exception_ptr error)
        : _contains(Contains::EXCEPTION)
        , _error(error)
    {
    }

private:
    Try(const Try&) = delete;
    Try& operator=(const Try&) = delete;

public:
    bool available() const
    {
        return _contains != Contains::NOTHING;
    }
    bool hasError() const
    {
        return _contains == Contains::EXCEPTION;
    }
    const T& value() const &
    {
        checkHasTry();
        return _value;
    }
    T& value() &
    {
        checkHasTry();
        return _value;
    }
    T&& value() &&
    {
        checkHasTry();
        return std::move(_value);
    }
    const T&& value() const &&
    {
        checkHasTry();
        return std::move(_value);
    }

    void setException(std::exception_ptr error)
    {
        if(_contains == Contains::EXCEPTION && _error == error)
        {
            return;
        }
        destroy();
        _contains = Contains::EXCEPTION;
        new (&_error) std::exception_ptr(error);
    }
    std::exception_ptr getException()
    {
        logicAssert(_contains == Contains::EXCEPTION, "Try object do not has an error");
        return _error;
    }

private:
    FL_INLINE void checkHasTry() const {
        if (FL_LIKELY(_contains == Contains::VALUE)) {
            return;
        } else if (_contains == Contains::EXCEPTION) {
            std::rethrow_exception(_error);
        } else if (_contains == Contains::NOTHING) {
            throw std::logic_error("Try object is empty");
        } else {
            assert(false);
        }
    }

    void destroy()
    {
        if (_contains == Contains::VALUE) {
            _value.~T();
        } else if (_contains == Contains::EXCEPTION) {
            _error.~exception_ptr();
        }
        _contains = Contains::NOTHING;
    }

private:
    Contains _contains = Contains::NOTHING;
    union {
        T _value;
        std::exception_ptr _error;
    };

private:
    friend Try<Unit>;
};

template <>
class Try<void> {
public:
    using value_type = void;

public:
    Try()
    {
    }
    Try(std::exception_ptr error) : _error(error) {}

    Try& operator=(std::exception_ptr error)
    {
        _error = error;
        return *this;
    }

public:
    Try(Try&& other) : _error(std::move(other._error)) {}
    Try& operator=(Try&& other) {
        if (this != &other) {
            std::swap(_error, other._error);
        }
        return *this;
    }

public:

    void value() 
    {
        if (_error) {
            std::rethrow_exception(_error);
        }
    }

    bool hasError() const { return _error.operator bool(); }

    void setException(std::exception_ptr error)
    {
        _error = error;
    }
    std::exception_ptr getException()
    {
        return _error;
    }

private:
    std::exception_ptr _error;
private:
    friend Try<Unit>;    
};

// T is Non void
template <typename F, typename... Args>
std::enable_if_t<!(std::is_same<invoke_result_t<F, Args...>, void>::value), Try<invoke_result_t<F, Args...>>>
makeTryCall(F&& f, Args&&... args)
{
    using T = invoke_result_t<F, Args...>;
    try {
        return Try<T>(std::forward<F>(f)(std::forward<Args>(args)...));
    } catch (...) {
        return Try<T>(std::current_exception());
    }
}

// T is void
template <typename F, typename... Args>
std::enable_if_t<std::is_same<invoke_result_t<F, Args...>, void>::value, Try<void>>
makeTryCall(F&& f, Args&&... args)
{
    try {
        std::forward<F>(f)(std::forward<Args>(args)...);
        return Try<void>();
    } catch (...) {
        return Try<void>(std::current_exception());
    }
}

template <typename TryType>
typename std::decay_t<TryType>::value_type& getTryValue(TryType&& t) {
    return t.value();
}
}

#endif //FUTURE_LITE_TRY_H
