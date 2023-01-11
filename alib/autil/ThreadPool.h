#ifndef AUTIL_THREADPOOL_H
#define AUTIL_THREADPOOL_H

#include <unistd.h>
#include "autil/Thread.h"
#include "autil/Lock.h"
#include "autil/WorkItem.h"
#include "autil/CircularQueue.h"
#include <vector>
#include <atomic>

AUTIL_BEGIN_NAMESPACE(autil);

class ThreadPool
{
public:
    enum {
        DEFAULT_THREADNUM = 4,
        DEFAULT_QUEUESIZE = 32
    };

    enum STOP_TYPE {
        STOP_THREAD_ONLY = 0,
        STOP_AND_CLEAR_QUEUE,
        STOP_AFTER_QUEUE_EMPTY
    };
    
    enum ERROR_TYPE {
        ERROR_NONE = 0,
        ERROR_POOL_HAS_STOP,
        ERROR_POOL_ITEM_IS_NULL,
        ERROR_POOL_QUEUE_FULL,
    };
public:
    ThreadPool(const size_t threadNum = DEFAULT_THREADNUM,
               const size_t queueSize = DEFAULT_QUEUESIZE);
    ~ThreadPool();
    
private:
    ThreadPool(const ThreadPool&);
    ThreadPool& operator = (const ThreadPool&);
public:
    ERROR_TYPE pushWorkItem(WorkItem *item, bool isBlocked = true);
    size_t getItemCount() const;
    size_t getThreadNum() const {return _threadNum;}
    size_t getQueueSize() const {return _queueSize;}
    bool start();
    bool start(const std::string &name);
    void stop(STOP_TYPE stopType = STOP_AFTER_QUEUE_EMPTY);
    void clearQueue();
    size_t getActiveThreadNum() const;
    const std::vector<ThreadPtr> &getThreads() const { return _threads; }
    typedef std::tr1::function<void ()> ThreadHook;
    void setThreadStartHook(const ThreadHook &hook) {
        _threadStartHook = hook;
    }
    void setThreadStopHook(const ThreadHook &hook) {
        _threadStopHook = hook;
    }
private:
    void push(WorkItem *item);
    void pushFront(WorkItem *item);
    bool tryPopItem(WorkItem* &);
    bool createThreads(const std::string &name);
    void workerLoop();
    void waitQueueEmpty();
    void stopThread();
private:
    size_t _threadNum;
    size_t _queueSize;
    CircularQueue<WorkItem *> _queue;
    std::vector<ThreadPtr> _threads;
    mutable ProducerConsumerCond _cond;
    volatile bool _push;
    volatile bool _run;
    std::atomic<std::uint32_t>  _activeThreadCount;
    ThreadHook _threadStartHook;
    ThreadHook _threadStopHook;
private:
    friend class ThreadPoolTest;
};

typedef std::tr1::shared_ptr<ThreadPool> ThreadPoolPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_THREADPOOL_H
