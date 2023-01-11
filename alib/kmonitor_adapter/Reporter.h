#pragma once

#include "kmonitor_adapter/ThreadData.h"
#include <autil/ThreadLocal.h>
#include <exception>
#include <mutex>
#include <atomic>
#include <unordered_set>
#include <thread>
#include <string>
#include <stdexcept>

namespace kmonitor_adapter {

class Recorder;

class Reporter {
private:
    Reporter();
    ~Reporter();

public:
    void registerRecorder(Recorder* recorder);
    void unregisterRecoder(Recorder* recorder);

    static Reporter* getInstance();

private:
    std::atomic<bool> _running;
    std::mutex _mtx;
    std::unordered_set<Recorder*> _recorders;
    std::thread _reportThread;

};

class Recorder {
public:
    Recorder();
    virtual ~Recorder();

    virtual void report() = 0;
    virtual const std::string& name() const {
        static std::string emptyString;
        return emptyString;
    }
    void registerRecorder();
    void unregisterRecoder();

    Recorder(const Recorder&) = delete;
    Recorder& operator=(const Recorder&) = delete;

public:
    static void unrefHandle(void* ptr) {
        if (ptr) {
            auto data = static_cast<AlignedThreadData*>(ptr);
            data->atExit();
            data->destroy();
        }
    }

public:
    virtual AlignedThreadData* createThreadData() = 0;

    __attribute__((__always_inline__)) inline AlignedThreadData* getThreadData() {
        auto threadDataPtr = static_cast<AlignedThreadData*>(_threadData->Get());
        if (__builtin_expect(threadDataPtr == nullptr, 0)) {
            threadDataPtr = createThreadData();
            _threadData->Reset(threadDataPtr);
        }
        return threadDataPtr;
    }

protected:
    std::unique_ptr<autil::ThreadLocalPtr> _threadData;
};

}  // namespace kmonitor_adapter

