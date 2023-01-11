#ifndef FUTURE_SIMPLE_EXECUTOR_H
#define FUTURE_SIMPLE_EXECUTOR_H

#include <unistd.h>
#include <functional>

#include <future_lite/Executor.h>
#include <future_lite/util/ThreadPool.h>
#include <future_lite/executors/SimpleIOExecutor.h>

#include <thread>

namespace future_lite {

namespace executors {

class SimpleExecutor : public Executor {
public:
    using Func = Executor::Func;
    using Context = Executor::Context;

    union ContextUnion {
        Context ctx;
        int64_t id;
    };

public:
    SimpleExecutor(size_t threadNum) : _pool(threadNum) {
        [[maybe_unused]] auto ret = _pool.start();
        assert(ret);
        _ioExecutor.init();
    }
    ~SimpleExecutor() {
        _ioExecutor.destroy();
    }

public:
    bool schedule(Func func) override {
        return _pool.scheduleById(std::move(func)) == util::ThreadPool::ERROR_NONE;
    }
    bool currentThreadInExecutor() const override { return _pool.getCurrentId() != -1; }
    ExecutorStat stat() const override { return ExecutorStat(); }

    size_t currentContextId() const override { return _pool.getCurrentId(); }

    Context checkout() override {
        ContextUnion u;
        u.id = _pool.getCurrentId();
        return u.ctx;
    }
    bool checkin(Func func, Context ctx) override {
        ContextUnion u;
        u.ctx = ctx;
        return _pool.scheduleById(std::move(func), u.id);
    }

    IOExecutor* getIOExecutor() override {
        return &_ioExecutor;
    }

private:
    util::ThreadPool _pool;
    SimpleIOExecutor _ioExecutor;
};

} // namespace executors

} // namespace future_lite

#endif
