#ifndef FUTURE_QUEUED_IMMEDIATE_EXECUTOR_H
#define FUTURE_QUEUED_IMMEDIATE_EXECUTOR_H

#include <unistd.h>
#include <functional>

#include <future_lite/Executor.h>
#include <future_lite/util/ThreadPool.h>
#include <autil/ThreadLocal.h>
#include <queue>

namespace future_lite {

namespace executors {

class QueuedImmediateExecutor : public Executor {
public:
    using Func = Executor::Func;
    using Context = Executor::Context;

    union ContextUnion {
        Context ctx;
        int64_t id;
    };

private:
    using Queue = std::queue<Func>;

private:
    QueuedImmediateExecutor()
        : _q([](void* entry) {
              if (entry) {
                  delete static_cast<Queue*>(entry);
              }
          }) {}

public:
    static QueuedImmediateExecutor* instance() {
        static QueuedImmediateExecutor executor;
        return &executor;
    }

public:
    bool schedule(Func func) override {
        auto& q = *getQueue();
        q.push(std::move(func));
        if (q.size() == 1) {
            while (!q.empty()) {
                q.front()();
                q.pop();
            }
        }
        return true;
    }
    bool currentThreadInExecutor() const override { return false; }
    ExecutorStat stat() const override { return ExecutorStat(); }

    size_t currentContextId() const override { return 0; }

    Context checkout() override { return 0; }
    bool checkin(Func func, Context ctx) override {
        (void)ctx;
        return schedule(std::move(func));
    }
    IOExecutor* getIOExecutor() override { return nullptr; }

private:
    Queue* getQueue() {
        auto q = _q.Get();
        if (unlikely(!q)) {
            q = new Queue();
            _q.Reset(q);
        }
        return static_cast<Queue*>(q);
    }

private:
    autil::ThreadLocalPtr _q;
};

} // namespace executors

} // namespace future_lite

#endif
