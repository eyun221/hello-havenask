#ifndef FSLIB_ASYNCFILEOPERATORIMPL_H
#define FSLIB_ASYNCFILEOPERATORIMPL_H

#include <iostream>
#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"
#include "fslib/fs/FileLock.h"
#include "fslib/fs/IOController.h"
#include "future_lite/IOExecutor.h"
#include "future_lite/Coroutines.h"

FSLIB_BEGIN_NAMESPACE(fs);

class AsyncFileOperatorImpl {
public:
    AsyncFileOperatorImpl() {}
    virtual ~AsyncFileOperatorImpl() {}

public:
    virtual bool await_ready() { return false; }
    virtual void await_resume() {}
    virtual void await_suspend(std::experimental::coroutine_handle<> handle) = 0;
};

FSLIB_END_NAMESPACE(fs);

#endif //FSLIB_ASYNCFILEOPERATORIMPL_H
