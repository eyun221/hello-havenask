#pragma once

#include "kmonitor/client/metric/Metric.h"

namespace kmonitor_adapter {

class LightMetric
{
public:
    LightMetric(kmonitor::Metric* metric)
        : _metric(metric) {
        assert(_metric);
        _metric->Acquire();
    }
    ~LightMetric() {
        _metric->Release();
        _metric = NULL;
    }

public:
    void report(double value) {
        assert(_metric);
        _metric->Update(value);
    }

private:
    kmonitor::Metric* _metric = NULL;
};
typedef std::unique_ptr<LightMetric> LightMetricPtr;

}

