#ifndef FUTURE_LITE_CORO_GENERATOR_H
#define FUTURE_LITE_CORO_GENERATOR_H

#include <cassert>
#include <exception>
#include <type_traits>
#include <utility>
#include <future_lite/experimental/coroutine.h>

namespace future_lite {
namespace coro {


// TODO(xinfei.sxf) exception && value -> Try
template <typename T>
class Generator {
public:
    class promise_type final {
    public:
        promise_type() noexcept
        : _value(nullptr),
            _exception(nullptr),
            _root(this),
            _parentOrLeaf(this) {}

        promise_type(const promise_type&) = delete;
        promise_type(promise_type&&) = delete;

        auto get_return_object() noexcept {
            return Generator<T>{*this};
        }

        std::experimental::suspend_always initial_suspend() noexcept {
            return {};
        }

        std::experimental::suspend_always final_suspend() noexcept {
            return {};
        }

        void unhandled_exception() noexcept {
            _exception = std::current_exception();
        }

        void return_void() noexcept {}

        std::experimental::suspend_always yield_value(T& value) noexcept {
            _value = std::addressof(value);
            return {};
        }

        std::experimental::suspend_always yield_value(T&& value) noexcept {
            _value = std::addressof(value);
            return {};
        }

        auto yield_value(Generator&& generator) noexcept {
            return yield_value(generator);
        }

        auto yield_value(Generator& generator) noexcept {
            struct awaitable {
                awaitable(promise_type* childPromise)
                    : _childPromise(childPromise) {}

                bool await_ready() noexcept {
                    return this->_childPromise == nullptr;
                }

                void await_suspend(
                    std::experimental::coroutine_handle<promise_type>) noexcept {}

                void await_resume() {
                    if (this->_childPromise != nullptr) {
                        this->_childPromise->throwIfException();
                    }
                }

            private:
                promise_type* _childPromise;
            };

            if (generator._promise != nullptr) {
                _root->_parentOrLeaf = generator._promise;
                generator._promise->_root = _root;
                generator._promise->_parentOrLeaf = this;
                generator._promise->resume();
                if (!generator._promise->isComplete()) {
                    return awaitable{generator._promise};
                }
                _root->_parentOrLeaf = this;
            }
            return awaitable{nullptr};
        }

        // Don't allow any use of 'co_await' inside the Generator
        // coroutine.
        template <typename U>
        std::experimental::suspend_never await_transform(U&& value) = delete;

        void destroy() noexcept {
            std::experimental::coroutine_handle<promise_type>::from_promise(*this)
                .destroy();
        }

        void throwIfException() {
            if (_exception != nullptr) {
                std::rethrow_exception(std::move(_exception));
            }
        }

        bool isComplete() noexcept {
            return std::experimental::coroutine_handle<promise_type>::from_promise(
                *this).done();
        }

        T& value() noexcept {
            assert(this == _root);
            assert(!isComplete());
            return *(_parentOrLeaf->_value);
        }

        void pull() noexcept {
            assert(this == _root);
            assert(!_parentOrLeaf->isComplete());
            _parentOrLeaf->resume();
            while (_parentOrLeaf != this && _parentOrLeaf->isComplete()) {
                _parentOrLeaf = _parentOrLeaf->_parentOrLeaf;
                _parentOrLeaf->resume();
            }
        }

    private:
        void resume() noexcept {
            std::experimental::coroutine_handle<promise_type>::from_promise(*this)
                .resume();
        }

        std::add_pointer_t<T> _value;
        std::exception_ptr _exception;
        promise_type* _root;

        // If this is the promise of the root generator then this field
        // is a pointer to the leaf promise.
        // For non-root generators this is a pointer to the parent promise.
        promise_type* _parentOrLeaf;
    };

    Generator() noexcept : _promise(nullptr) {}

    Generator(promise_type& promise) noexcept : _promise(&promise) {}

    Generator(Generator&& other) noexcept : _promise(other._promise) {
        other._promise = nullptr;
    }

    Generator(const Generator& other) = delete;
    Generator& operator=(const Generator& other) = delete;

    ~Generator() {
        if (_promise != nullptr) {
            _promise->destroy();
        }
    }

    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (_promise != nullptr) {
                _promise->destroy();
            }
            _promise = other._promise;
            other._promise = nullptr;
        }

        return *this;
    }

    class iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        // What type should we use for counting elements of a potentially infinite
        // sequence?
        using difference_type = std::ptrdiff_t;
        using value_type = std::remove_reference_t<T>;
        using reference = std::conditional_t<std::is_reference<T>::value, T, T&>;
        using pointer = std::add_pointer_t<T>;

        iterator() noexcept : _promise(nullptr) {}

        explicit iterator(promise_type* promise) noexcept : _promise(promise) {}

        bool operator==(const iterator& other) const noexcept {
            return _promise == other._promise;
        }

        bool operator!=(const iterator& other) const noexcept {
            return _promise != other._promise;
        }

        iterator& operator++() {
            assert(_promise != nullptr);
            assert(!_promise->isComplete());
            _promise->pull();
            if (_promise->isComplete()) {
                auto* temp = _promise;
                _promise = nullptr;
                temp->throwIfException();
            }
            return *this;
        }

        void operator++(int) {
            (void)operator++();
        }

        reference operator*() const noexcept {
            assert(_promise != nullptr);
            return static_cast<reference>(_promise->value());
        }

        pointer operator->() const noexcept {
            return std::addressof(operator*());
        }

    private:
        promise_type* _promise;
    };

    iterator begin() {
        if (_promise != nullptr) {
            _promise->pull();
            if (!_promise->isComplete()) {
                return iterator(_promise);
            }
            _promise->throwIfException();
        }
        return iterator(nullptr);
    }

    iterator end() noexcept {
        return iterator(nullptr);
    }

    void swap(Generator& other) noexcept {
        std::swap(_promise, other._promise);
    }

private:
    friend class promise_type;
    promise_type* _promise;
};

template <typename T>
void swap(Generator<T>& a, Generator<T>& b) noexcept {
    a.swap(b);
}

}  // namespace coro
}  // namespace future_lite

#endif
