#ifndef FUTURE_LITE_IO_EXECUTOR_H
#define FUTURE_LITE_IO_EXECUTOR_H


#include <functional>
#include <string>
#include <libaio.h>

namespace future_lite {

enum iocb_cmd {
    IOCB_CMD_PREAD = 0,
    IOCB_CMD_PWRITE = 1,
    IOCB_CMD_FSYNC = 2,
    IOCB_CMD_FDSYNC = 3,
    /* These two are experimental.
     * IOCB_CMD_PREADX = 4,
     * IOCB_CMD_POLL = 5,
     */
    IOCB_CMD_NOOP = 6,
    IOCB_CMD_PREADV = 7,
    IOCB_CMD_PWRITEV = 8,
};

using AIOCallback = std::function<void(io_event&)>;

class IOExecutor {
public:
    using Func = std::function<void()>;

    IOExecutor() {} 
    virtual ~IOExecutor() {}

    IOExecutor(const IOExecutor &) = delete;
    IOExecutor& operator = (const IOExecutor &) = delete;

public:
    virtual void submitIO(int fd, iocb_cmd cmd, void* buffer, size_t length, off_t offset,
                               AIOCallback cbfn) = 0;
    virtual void submitIOV(int fd, iocb_cmd cmd, const iovec* iov, size_t count, off_t offset,
                                AIOCallback cbfn) = 0;
    
    // add stat info
    // virtual IoExecutorStat stat() const = 0;
};

}  // namespace future_lite

#endif
