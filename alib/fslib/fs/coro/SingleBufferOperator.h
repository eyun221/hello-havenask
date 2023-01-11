#ifndef FSLIB_CORO_SINGLE_BUFFER_OPERATOR_H
#define FSLIB_CORO_SINGLE_BUFFER_OPERATOR_H

#include <iostream>
#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"
#include "fslib/fs/FileLock.h"
#include "fslib/fs/IOController.h"
#include "fslib/fs/coro/AsyncFileOperatorImpl.h"
#include <future_lite/IOExecutor.h>
#include <future_lite/Coroutines.h>

FSLIB_BEGIN_NAMESPACE(fs);

class File;

class SingleBufferOperator final : public AsyncFileOperatorImpl {
public:
    SingleBufferOperator(IOController* controller, future_lite::iocb_cmd cmd, File* file,
                         void* buffer, size_t length, off_t offset)
        : _controller(controller),
          _cmd(cmd),
          _file(file),
          _buffer(buffer),
          _length(length),
          _offset(offset) {}

public:
    void await_suspend(std::experimental::coroutine_handle<> handle) override {
        const auto& pr = static_cast<std::experimental::coroutine_handle<
            future_lite::coro::detail::LazyPromiseBase>*>(&handle)
                             ->promise();
        _controller->setExecutor(pr._executor);
        std::function<void()> callback = [h = handle]() mutable { h.resume(); };
        if (_cmd == future_lite::IOCB_CMD_PREAD) {
            _file->pread(_controller, _buffer, _length, _offset, std::move(callback));
        } else if (_cmd == future_lite::IOCB_CMD_PWRITE) {
            _file->pwrite(_controller, _buffer, _length, _offset, std::move(callback));
        }
    }

protected:
    IOController* _controller;
    future_lite::iocb_cmd _cmd;
    File* _file;
    void* _buffer;
    size_t _length;
    off_t _offset;
};

FSLIB_END_NAMESPACE(fs);

#endif
