#ifndef FUTURE_LITE_CORO_TRAITS_H
#define FUTURE_LITE_CORO_TRAITS_H


#include <future_lite/Common.h>
#include <future_lite/Invoke.h>
#include <exception>
#include <utility>

namespace future_lite {

namespace coro {

namespace detail {

template <typename T>
class HasCoAwaitMethod{
    template <typename C>
    static int8_t test(decltype(std::declval<C>().coAwait(nullptr))*);
    
    template <typename C>
    static int16_t test(...);

public:
    static constexpr bool value = (sizeof(test<T>(nullptr)) == sizeof(int8_t));
};

template <typename T>
class HasMemberCoAwaitOperator {
    template <typename C>
    static int8_t test(decltype(std::declval<C>().operator co_await())*);
    template <typename C>
    static int16_t test(...);
public:
    static constexpr bool value = (sizeof(test<T>(nullptr)) == sizeof(int8_t));
};

template <typename T>
class HasGlobalCoAwaitOperator {
    template <typename C>
    static int8_t test(decltype(operator co_await( std::declval<C>()))*);
    template <typename C>
    static int16_t test(...);
public:
    static constexpr bool value = (sizeof(test<T>(nullptr)) == sizeof(int8_t));
};

template <typename Awaitable,
          std::enable_if_t<HasMemberCoAwaitOperator<std::decay_t<Awaitable>>::value, int> = 0>
auto getAwaiter(Awaitable&& awaitable) {
    return std::forward<Awaitable>(awaitable).operator co_await();
}

template <typename Awaitable,
          std::enable_if_t<HasGlobalCoAwaitOperator<std::decay_t<Awaitable>>::value, int> = 0>
auto getAwaiter(Awaitable&& awaitable) {
    return operator co_await(std::forward<Awaitable>(awaitable));
}

template <typename Awaitable,
          std::enable_if_t<!HasMemberCoAwaitOperator<std::decay_t<Awaitable>>::value, int> = 0,
          std::enable_if_t<!HasGlobalCoAwaitOperator<std::decay_t<Awaitable>>::value, int> = 0>
auto getAwaiter(Awaitable&& awaitable) {
    return std::forward<Awaitable>(awaitable);
}

}  // namespace detail

}  // namespace coro
}  // namespace future_lite



#endif
