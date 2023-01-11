#ifndef FUTURE_LITE_CORO_COLLECT_H
#define FUTURE_LITE_CORO_COLLECT_H

#include <future_lite/Common.h>
#include <future_lite/Invoke.h>
#include <future_lite/experimental/coroutine.h>
#include <future_lite/coro/Event.h>
#include <future_lite/Try.h>
#include <future_lite/coro/Lazy.h>
#include <exception>
#include <vector>

namespace future_lite {
namespace coro {

// TODO: add more collectAll types
//  eg:  auto [x, y] = co_await collectAll(IntLazy, FloatLazy);

namespace detail {

template <typename T>
struct CollectAnyResult {
    CollectAnyResult() : _idx(static_cast<size_t>(-1)), _value() {}
    CollectAnyResult(size_t idx, T&& value)
        : _idx(idx), _value(std::move(value)) {}

    CollectAnyResult(const CollectAnyResult&) = delete;
    CollectAnyResult& operator=(const CollectAnyResult&) = delete;
    CollectAnyResult(CollectAnyResult&& other)
        : _idx(std::move(other._idx)), _value(std::move(other._value)) {
        other._idx = static_cast<size_t>(-1);
    }

    size_t _idx;
    Try<T> _value;
};

template<>
struct CollectAnyResult<void> {
    CollectAnyResult() : _idx(static_cast<size_t>(-1)) {}
    size_t _idx;
    Try<void> _value;
};

template <typename LazyType, typename InAlloc>
struct CollectAnyAwaiter {
    using ValueType = typename LazyType::ValueType;
    using ResultType = CollectAnyResult<ValueType>;

    CollectAnyAwaiter(std::vector<LazyType, InAlloc>&& input)
        : _input(std::move(input)), _result(nullptr) {}

    CollectAnyAwaiter(const CollectAnyAwaiter&) = delete;
    CollectAnyAwaiter& operator=(const CollectAnyAwaiter&) = delete;
    CollectAnyAwaiter(CollectAnyAwaiter&& other)
        : _input(std::move(other._input)), _result(std::move(other._result)) {}

    bool await_ready() const noexcept {
        return _input.empty() || (_result && _result->_idx != -1);
    }

    void await_suspend(std::experimental::coroutine_handle<> continuation) {
        auto promise_type =
            static_cast<std::experimental::coroutine_handle<LazyPromiseBase>*>(
                &continuation)
                ->promise();
        auto executor = promise_type._executor;
        // Make local copies to shared_ptr to avoid deleting objects too early
        // if any coroutine finishes before this function.
        std::vector<LazyType, InAlloc> input(std::move(_input));
        auto result = std::make_shared<ResultType>();
        auto event = std::make_shared<detail::CountEvent>(input.size());
        size_t i = 0;

        _result = result;
        for (i = 0; i < input.size() && (result->_idx == -1); ++i) {
            if (!input[i]._coro.promise()._executor) {
                input[i]._coro.promise()._executor = executor;
            }

            input[i].start([i = i, size = input.size(), r = result,
                            c = continuation,
                            e = event](Try<ValueType>&& result) mutable {
                assert(e != nullptr);
                auto count = e->downCount();
                if (count == size + 1) {
                    r->_idx = i;
                    r->_value = std::move(result);
                    c.resume();
                }
            });
        }  // end for
    }
    auto await_resume() {
        assert(_result != nullptr);
        return std::move(*_result);
    }

    std::vector<LazyType, InAlloc> _input;
    std::shared_ptr<ResultType> _result;
};

template <typename T, typename InAlloc>
struct SimpleCollectAnyAwaitable {
    using ValueType = T;
    using LazyType = Lazy<T>;
    using VectorType = std::vector<LazyType, InAlloc>;

    VectorType _input;

    SimpleCollectAnyAwaitable(std::vector<LazyType, InAlloc>&& input)
        : _input(std::move(input)) {}

    auto coAwait(Executor* ex) {
        return CollectAnyAwaiter<LazyType, InAlloc>(std::move(_input));
    }
};

template <typename LazyType, typename IAlloc, typename OAlloc>
struct CollectAllAwaiter {
    using ValueType = typename LazyType::ValueType;

    CollectAllAwaiter(std::vector<LazyType, IAlloc>&& input, OAlloc outAlloc)
        : _input(std::move(input)), _output(outAlloc), _event(_input.size()) {
            _output.resize(_input.size());
    }
    CollectAllAwaiter(CollectAllAwaiter&& other)
        : _input(std::move(other._input)),
          _output(std::move(other._output)),
          _event(std::move(other._event)) {}

    CollectAllAwaiter(const CollectAllAwaiter&) = delete;
    CollectAllAwaiter& operator=(const CollectAllAwaiter&) = delete;

    inline bool await_ready() const noexcept { return _input.empty(); }
    inline void await_suspend(
        std::experimental::coroutine_handle<> continuation) {
        auto promise_type =
            static_cast<std::experimental::coroutine_handle<LazyPromiseBase>*>(
                &continuation)
                ->promise();
        auto executor = promise_type._executor;

        for (size_t i = 0; i < _input.size(); ++i) {
            if (!_input[i]._coro.promise()._executor) {
                _input[i]._coro.promise()._executor = executor;
            }
            _input[i].start([this, i](Try<ValueType>&& result) {
                _output[i] = std::move(result);
                auto awaitingCoro = _event.down();
                if (awaitingCoro) {
                    awaitingCoro.resume();
                }
            });
        }
        _event.setAwaitingCoro(continuation);
        auto awaitingCoro = _event.down();
        if (awaitingCoro) {
            awaitingCoro.resume();
        }
    }
    inline auto await_resume() { return std::move(_output); }

    std::vector<LazyType, IAlloc> _input;
    std::vector<Try<ValueType>, OAlloc> _output;
    detail::CountEvent _event;
};  // CollectAllAwaiter

template <typename T, typename IAlloc, typename OAlloc>
struct SimpleCollectAllAwaitable {
    using ValueType = T;
    using LazyType = Lazy<T>;
    using VectorType = std::vector<LazyType, IAlloc>;

    VectorType _input;
    OAlloc _out_alloc;

    SimpleCollectAllAwaitable(VectorType&& input, OAlloc out_alloc)
        : _input(std::move(input)), _out_alloc(out_alloc) {}

    auto coAwait(Executor* ex) {
        return CollectAllAwaiter<LazyType, IAlloc, OAlloc>(
            std::move(_input), _out_alloc);
    }
};  // SimpleCollectAllAwaitable

}  // namespace detail

// collectAny
template<typename T, template<typename> typename LazyType,
         typename IAlloc = std::allocator<LazyType<T>>>
inline auto collectAny(std::vector<LazyType<T>, IAlloc>&& input)
    -> Lazy<detail::CollectAnyResult<T>> {
    using AT = std::conditional_t<std::is_same_v<LazyType<T>, Lazy<T>>,
                   detail::SimpleCollectAnyAwaitable<T, IAlloc>,
                   detail::CollectAnyAwaiter<LazyType<T>, IAlloc>>;
    co_return co_await AT(std::move(input));
}

// collectAll
template <typename T, template<typename> typename LazyType,
          typename IAlloc = std::allocator<LazyType<T>>,
          typename OAlloc = std::allocator<Try<T>>>
inline auto collectAll(std::vector<LazyType<T>, IAlloc>&& input,
                       OAlloc out_alloc = OAlloc())
    -> Lazy<std::vector<Try<T>, OAlloc>> {
    using AT = std::conditional_t<std::is_same_v<LazyType<T>, Lazy<T>>,
                   detail::SimpleCollectAllAwaitable<T, IAlloc, OAlloc>,
                   detail::CollectAllAwaiter<LazyType<T>, IAlloc, OAlloc>>;
    co_return co_await AT(std::move(input), out_alloc);
}

// batched collectAll
template <typename T, template<typename> typename LazyType,
          typename IAlloc = std::allocator<LazyType<T>>,
          typename OAlloc = std::allocator<Try<T>>>
inline auto collectAll(size_t batch, std::vector<LazyType<T>, IAlloc>&& input,
                       OAlloc out_alloc = OAlloc())
    -> Lazy<std::vector<Try<T>, OAlloc>> {
    using AT = std::conditional_t<std::is_same_v<LazyType<T>, Lazy<T>>,
                   detail::SimpleCollectAllAwaitable<T, IAlloc, OAlloc>,
                   detail::CollectAllAwaiter<LazyType<T>, IAlloc, OAlloc>>;
    std::vector<Try<T>, OAlloc> output(out_alloc);
    size_t input_size = input.size();
    // batch == 0;
    // input_size <= batch size;
    // act just like CollectAll.
    if (batch == 0 || input_size <= batch) {
        co_return co_await AT(std::move(input), out_alloc);
    }
    size_t start = 0;
    while (start < input_size) {
        size_t end = std::min(input_size, start + batch);
        std::vector<LazyType<T>, IAlloc> tmp_group(input.get_allocator());
        for (; start < end; ++start) {
            tmp_group.push_back(std::move(input[start]));
        }
        auto tmp_output = co_await AT(std::move(tmp_group), out_alloc);
        for (auto& t : tmp_output) {
            output.push_back(std::move(t));
        }
    }
    co_return std::move(output);
}

// variadic collectAll
template <template <typename> typename LazyType, typename Ts>
Lazy<void> makeWraperTask(LazyType<Ts>&& awaitable, Try<Ts>& result) {
    result = co_await awaitable.coAwaitTry();
}

template <template <typename> typename LazyType, typename... Ts,
          size_t... Indices>
inline auto collectAllVariadicImpl(std::index_sequence<Indices...>,
                                   LazyType<Ts>&&... awaitables)
    -> Lazy<std::tuple<Try<Ts>...>> {
    static_assert(sizeof...(Ts) > 0);

    std::tuple<Try<Ts>...> results;
    std::vector<Lazy<void>> wraper_tasks;

    // make wraper task
    (void)std::initializer_list<bool>{
        (wraper_tasks.push_back(std::move(makeWraperTask(
             std::move(awaitables), std::get<Indices>(results)))),
         true)...,
    };

    co_await collectAll(std::move(wraper_tasks));
    co_return std::move(results);
}

template <template <typename> typename LazyType, typename... Ts>
inline auto collectAll(LazyType<Ts>&&... inputs)
    -> Lazy<std::tuple<Try<Ts>...>> {
    if constexpr (0 == sizeof...(Ts)) {
        co_return std::tuple<>{};
    } else {
        co_return co_await collectAllVariadicImpl(
            std::make_index_sequence<sizeof...(Ts)>{}, std::move(inputs)...);
    }
}

}  // namespace coro
}  // namespace future_lite

#endif
