#ifndef FUTURE_LITE_EIGEN_EXECUTOR_H
#define FUTURE_LITE_EIGEN_EXECUTOR_H

#include <functional>
#include <thread>
#include <stdexcept>
#include "future_lite/Executor.h"
#include "future_lite/IOExecutor.h"
#include "third_party/eigen3/unsupported/Eigen/CXX11/ThreadPool" // TODO: This file is under MPL 2.0

namespace future_lite {

struct EigenEnvironment
{
    struct Thread {
        std::thread thread;

        template <typename F>
        explicit Thread(F&& func)
            : thread(std::forward<F>(func))
        {
        }
        ~Thread()
        {
            thread.join();
        }
    };
    typedef Thread EnvThread;
    struct TaskImpl {
        future_lite::Executor::Func func;
    };
    struct Task
    {
        std::unique_ptr<TaskImpl> f;
    };

    EnvThread* CreateThread(std::function<void()> f)
    {
        return new Thread(std::move(f));
    }
    Task CreateTask(std::function<void()> f)
    {
        std::unique_ptr<TaskImpl> taskImpl(new TaskImpl { std::move(f) });
        return Task { std::move(taskImpl) };
    }
    void ExecuteTask(const Task& t)
    {
        t.f->func();
    }
    EigenEnvironment() = default;
};

class EigenExecutor final : public Executor {
public:
    EigenExecutor(size_t threadNum, std::string name = "default")
        : Executor(std::move(name)), _impl(threadNum, EigenEnvironment()){};
    ~EigenExecutor() {};

public:
    bool schedule(Func func) override
    {
        _impl.Schedule(std::move(func));
        return true;
    }
    bool currentThreadInExecutor() const override
    {
        return _impl.CurrentThreadId() != -1;
    }

    ExecutorStat stat() const override
    {
        ExecutorStat stat;
        stat.pendingTaskCount = _impl.PendingTaskCount();
        return stat;
    }
    
    IOExecutor* getIOExecutor() override {
        throw std::runtime_error("unsupported getIOExecutor function in this Executor");
        return NULL;
    }

private:
    Eigen::NonBlockingThreadPoolTempl<EigenEnvironment> _impl;
};
}

#endif
