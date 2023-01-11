#ifndef FSLIB_ASYNCFILEOPERATOR_H
#define FSLIB_ASYNCFILEOPERATOR_H

#include <iostream>
#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"
#include "fslib/fs/FileLock.h"
#include "future_lite/Coroutines.h"

FSLIB_BEGIN_NAMESPACE(fs);

class AsyncFileOperatorImpl;

class AsyncFileOperator {
public:
#if FUTURE_LITE_USE_COROUTINES
    AsyncFileOperator(AsyncFileOperatorImpl* impl) : _impl(impl) {}
    ~AsyncFileOperator();

    AsyncFileOperator& operator=(const AsyncFileOperator& other) = delete;
    AsyncFileOperator(const AsyncFileOperator& other) = delete;

    AsyncFileOperator(AsyncFileOperator&& other) : _impl(other._impl) {
        other._impl = nullptr;
    }

    AsyncFileOperator& operator=(AsyncFileOperator&& other) {
        if (this != &other) {
            std::swap(_impl, other._impl);
        }
        return *this;
    }

    bool await_ready();
    void await_suspend(std::experimental::coroutine_handle<> handle);
    void await_resume();

private:
    AsyncFileOperatorImpl* _impl;
#endif
};

FSLIB_END_NAMESPACE(fs);

#endif //FSLIB_ASYNCFILEOPERATOR_H
