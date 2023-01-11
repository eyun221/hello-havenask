#ifndef AUTIL_LOOPTHREAD_H
#define AUTIL_LOOPTHREAD_H

#include "autil/CommonMacros.h"
#include "autil/Thread.h"
#include "autil/Lock.h"

AUTIL_BEGIN_NAMESPACE(autil);

class LoopThread;
typedef std::tr1::shared_ptr<LoopThread> LoopThreadPtr;

class LoopThread
{
private:
    LoopThread();
public:
    ~LoopThread();
private:
    LoopThread(const LoopThread &);
    LoopThread& operator = (const LoopThread &);
public:
    void stop();
public:
    // if create loop thread fail, return null LoopThreadPtr
    static LoopThreadPtr createLoopThread(const std::tr1::function<void ()> &loopFunction, int64_t loopInterval /*us*/);
    static LoopThreadPtr createLoopThread(const std::tr1::function<void ()> &loopFunction, int64_t loopInterval /*us*/, const std::string &name);
    void runOnce();
private:
    void loop();
private:
    volatile bool _run;
    ThreadCond _cond;
    ThreadPtr _threadPtr;
    std::tr1::function<void ()> _loopFunction;
    int64_t _loopInterval;
    int64_t _nextTime;
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_LOOPTHREAD_H
