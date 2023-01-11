#ifndef FSLIB_CORO_MULTI_BUFFER_OPERATOR_H
#define FSLIB_CORO_MULTI_BUFFER_OPERATOR_H

#include <sys/types.h>
#include <sys/uio.h>
#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"
#include "fslib/fs/FileLock.h"
#include "fslib/fs/IOController.h"
#include "fslib/fs/coro/AsyncFileOperatorImpl.h"
#include "future_lite/IOExecutor.h"
#include "future_lite/experimental/coroutine.h"

FSLIB_BEGIN_NAMESPACE(fs);

class File;

class MultiBufferOperator final : public AsyncFileOperatorImpl {
public:
    MultiBufferOperator(IOController* controller, future_lite::iocb_cmd cmd, File* file,
                        const iovec* iov, int iovcnt, off_t offset)
        : _controller(controller),
          _cmd(cmd),
          _file(file),
          _iov(iov),
          _iovcnt(iovcnt),
          _offset(offset) {}

public:
    void await_suspend(std::experimental::coroutine_handle<> handle) override {
        const auto& pr = static_cast<std::experimental::coroutine_handle<
            future_lite::coro::detail::LazyPromiseBase>*>(&handle)
                             ->promise();
        _controller->setExecutor(pr._executor);
        std::function<void()> callback = [h = handle]() mutable { h.resume(); };
        if (_cmd == future_lite::IOCB_CMD_PREADV) {
            _file->preadv(_controller, _iov, _iovcnt, _offset, std::move(callback));
        } else if (_cmd == future_lite::IOCB_CMD_PWRITEV) {
            _file->pwritev(_controller, _iov, _iovcnt, _offset, std::move(callback));
        }
    }

protected:
    IOController* _controller;
    future_lite::iocb_cmd _cmd;
    File* _file;
    const iovec* _iov;
    int _iovcnt;
    off_t _offset;
};

FSLIB_END_NAMESPACE(fs);

#endif
