#pragma once

#include "kmonitor_adapter/Reporter.h"
#include "kmonitor_adapter/ThreadData.h"
#include "kmonitor_adapter/MonitorMetric.h"
#include "kmonitor_adapter/Monitor.h"
#include <atomic>

namespace kmonitor_adapter {

class QpsRecorder : public Recorder {
private:
    struct ThreadCounter : public AlignedThreadData {
        std::atomic_int_fast64_t value;
        // During teardown, value will be summed into *merged_sum.
        std::atomic_int_fast64_t* mergedSum;
        ThreadCounter(int64_t _value,
                      std::atomic_int_fast64_t* _mergedSum)
            : value(_value)
            , mergedSum(_mergedSum)
        {}

        void atExit() override { *mergedSum += value.load(); }
    };

public:
    QpsRecorder(const std::string& name, MetricPtr metric);
    QpsRecorder(Monitor* monitor, const std::string& metricName, kmonitor::MetricLevel level,
                const Metric::KVVec& tags = Metric::KVVec());
    ~QpsRecorder();

    const std::string& name() const override { return _name; }

    void record(int64_t val) {
        auto threadCounter = static_cast<ThreadCounter*>(getThreadData());
        assert(threadCounter);
        threadCounter->value.fetch_add(val, std::memory_order_relaxed);
    }
    void report() override {
        if (!_metric){
            return;
        }
        int64_t threadLocalSum = 0;
        _threadData->Fold(
            [](void* entryPtr, void* res) {
                auto* sumPtr = static_cast<int64_t*>(res);
                auto threadCounter = static_cast<ThreadCounter*>(entryPtr);
                *sumPtr += threadCounter->value.load(std::memory_order_relaxed);
            },
            &threadLocalSum);
        auto cur = threadLocalSum + _mergedSum.load(std::memory_order_relaxed);
        _metric->report(cur - _last);
        _last = cur;
    }

private:
    AlignedThreadData* createThreadData() override {
        return new ThreadCounter(0, &_mergedSum);
    }

private:
    std::string _name;
    MetricPtr _metric;

    // counter
    std::atomic_int_fast64_t _mergedSum;

    int64_t _last = 0;

};

}  // namespace kmonitor_adapter

