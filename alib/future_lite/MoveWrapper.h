#ifndef FUTURE_LITE_MOVEWRAPPER_H
#define FUTURE_LITE_MOVEWRAPPER_H


#include <future_lite/Common.h>
#include <exception>

namespace future_lite {

// std::function requre copyConstructable, hence we provide MoveWrapper perform copy as move

template<typename T>
class MoveWrapper {
public:
    MoveWrapper() = default;
    MoveWrapper(T&& value)
        : _value(std::move(value))
    {
    }

    MoveWrapper(const MoveWrapper& other)
        : _value(std::move(other._value))
    {
    }
    MoveWrapper(MoveWrapper&& other)
        : _value(std::move(other._value))
    {
    }    

    MoveWrapper& operator=(const MoveWrapper& ) = delete;
    MoveWrapper& operator=(MoveWrapper&&) = delete;

    T& get()
    {
        return _value;
    }
    const T& get() const
    {
        return _value;
    }

    ~MoveWrapper() {}

private:
    mutable T _value;
};

}

#endif //FUTURE_LITE_VALUE_H
