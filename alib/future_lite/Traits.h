#ifndef FUTURE_LITE_TRAITS_H
#define FUTURE_LITE_TRAITS_H


#include <future_lite/Common.h>
#include <future_lite/Try.h>
#include <future_lite/Unit.h>
#include <exception>

namespace future_lite {

template <typename T>
class Future;

template <typename T>
struct IsFuture : std::false_type {
    using Inner = T;
};

template <>
struct IsFuture<void> : std::false_type {
    using Inner = Unit;
};

template <typename T>
struct IsFuture<Future<T>> : std::true_type {
    using Inner = T;
};

template <typename T, typename F>
struct TryCallableResult {
    using Result = invoke_result_t<F, Try<T>&&>;
    using ReturnsFuture = IsFuture<Result>;
    static constexpr bool isTry = true;
};

template <typename T, typename F>
struct ValueCallableResult {
    using Result = invoke_result_t<F, T&&>;
    using ReturnsFuture = IsFuture<Result>;
    static constexpr bool isTry = false;    
};

namespace detail {
template <typename T>
struct remove_cvref {
    using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;
}  // namespace detail

}  // namespace future_lite

#endif //FUTURE_LITE_TRAITS_H
