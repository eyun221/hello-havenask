#ifndef ANET_COND_H_
#define ANET_COND_H_
#include <anet/threadmutex.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>
namespace anet {

class ThreadCond : public ThreadMutex {

public:

    ThreadCond() {
        pthread_cond_init(&_cond, NULL);
    }

    ~ThreadCond() {
        pthread_cond_destroy(&_cond);
    }

    /*
     * wait
     *
     * @param  milliseconds, 0 = never timeout
     */
    bool wait(int milliseconds = 0) {
        bool ret = true;

        if (milliseconds == 0) { // never timeout
            pthread_cond_wait(&_cond, &_mutex);
        } else {

            struct timeval curtime;

            struct timespec abstime;
            gettimeofday(&curtime, NULL);

            int64_t us = (static_cast<int64_t>(curtime.tv_sec) *
                          static_cast<int64_t>(1000000) +
                          static_cast<int64_t>(curtime.tv_usec) +
                          static_cast<int64_t>(milliseconds) *
                          static_cast<int64_t>(1000));

            abstime.tv_sec = static_cast<int>(us / static_cast<int64_t>(1000000));
            abstime.tv_nsec = static_cast<int>(us % static_cast<int64_t>(1000000)) * 1000;
            ret = (pthread_cond_timedwait(&_cond, &_mutex, &abstime) == 0);
        }

        return ret;
    }

    void signal() {
        pthread_cond_signal(&_cond);
    }

    void broadcast() {
        pthread_cond_broadcast(&_cond);
    }

private:
    pthread_cond_t _cond;
};

}

#endif /*COND_H_*/
