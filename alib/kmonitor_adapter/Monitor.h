#pragma once

#include <autil/Log.h>
#include <memory>
#include <vector>
#include <autil/Lock.h>
#include <autil/TimeUtility.h>
#include <map>
#include "kmonitor/client/common/Log.h"
#include "kmonitor/client/MetricLevel.h"
#include "kmonitor/client/core/MutableMetric.h"
#include "MonitorMetric.h"

namespace kmonitor_adapter {

class ScopeLatencyReporter {
public:
    ScopeLatencyReporter(MetricPtr &metric)
        : _beginTime(autil::TimeUtility::currentTime())
        , _metric(metric)
    {}
    ~ScopeLatencyReporter() {
        if (_metric) { 
            int64_t endTime = autil::TimeUtility::currentTime();
            _metric->report((endTime - _beginTime) / 1000.0);
        }
    }
private:
    int64_t _beginTime;
    MetricPtr &_metric;
};
    
class Monitor {
 public:
    Monitor(std::string serviceName, autil::RecursiveThreadMutex* metricMutex,
            kmonitor::KMonitor* kmonitor);
    ~Monitor();

    void init(std::string service);

    typedef std::vector<std::pair<std::string, std::string>> KVVec;
    MetricPtr registerRawMetric(const std::string& name, kmonitor::MetricLevel level,
                                const KVVec& tags = KVVec());
    MetricPtr registerQPSMetric(const std::string& name, kmonitor::MetricLevel level,
                                const KVVec& tags = KVVec());
    MetricPtr registerStatusMetric(const std::string& name, kmonitor::MetricLevel level,
                                   const KVVec& tags = KVVec());
    MetricPtr registerCounterMetric(const std::string& name, kmonitor::MetricLevel level,
                                    const KVVec& tags = KVVec());
    MetricPtr registerGaugeMetric(const std::string& name, kmonitor::MetricLevel level,
                                  const KVVec& tags = KVVec(), bool more_stats = false);
    MetricPtr registerGaugeSumMetric(const std::string& name, kmonitor::MetricLevel level,
                                     const KVVec& tags = KVVec());
    MetricPtr registerGaugeMaxMetric(const std::string& name, kmonitor::MetricLevel level,
                                     const KVVec& tags = KVVec());
    // max, p95, p99, p999
    MetricPtr registerGaugePercentileMetric(const std::string& name, kmonitor::MetricLevel level,
                                            const KVVec& tags = KVVec());
    // max, p99(global + local)
    MetricPtr registerLatencyMetric(const std::string& name, kmonitor::MetricLevel level,
                                    const KVVec& tags = KVVec());
    void unregisterMetric(const std::string& name);

    MetricPtr registerMetric(const std::string& name, kmonitor::MetricType metricType,
                             int statisticsType, kmonitor::MetricLevel level,
                             const KVVec& tags = KVVec());

 private:
    autil::RecursiveThreadMutex* _metricMutex;
    kmonitor::KMonitor* _kmonitor = nullptr;
};

typedef std::unique_ptr<Monitor> MonitorPtr;

}  // namespace kmonitor_adapter

