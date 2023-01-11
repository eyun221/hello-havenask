#ifndef AUTIL_THREAD_H
#define AUTIL_THREAD_H

#include "autil/CommonMacros.h"
#include <pthread.h>
#include <tr1/memory>
#include <tr1/functional>
#include <cassert>

AUTIL_BEGIN_NAMESPACE(autil);

class Thread;
typedef std::tr1::shared_ptr<Thread> ThreadPtr;

class Thread
{
public:
    // when create thread fail, return null ThreadPtr
    // need hold return value else will hang to the created thread terminate
    static ThreadPtr createThread(const std::tr1::function<void ()>& threadFunction);
    static ThreadPtr createThread(const std::tr1::function<void ()>& threadFunction,
                                  const std::string &name);
private:
    static void* threadWrapperFunction(void* arg);
public:
    pthread_t getId() const {return _id;}
    void join() {
        if (_id) {
            int ret = pthread_join(_id, NULL);
            (void) ret; assert(ret == 0);
        }
        _id = 0;
    }
public:
    ~Thread() {
        join();
    }
private:
    Thread() {_id = 0;}
    pthread_t _id;
};

AUTIL_END_NAMESPACE(autil);

#endif // AUTIL_THREAD_H
