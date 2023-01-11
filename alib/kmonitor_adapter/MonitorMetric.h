#pragma once

#include "kmonitor/client/core/MetricsTags.h"
#include "kmonitor/client/KMonitor.h"
#include "kmonitor/client/MetricLevel.h"
#include "kmonitor/client/core/MutableMetric.h"
#include "kmonitor_adapter/LightMetric.h"

namespace kmonitor_adapter {

class Metric;
typedef std::unique_ptr<Metric> MetricPtr;

class Metric
{
public:
    Metric(kmonitor::MutableMetric* metric)
        : _metric(metric)
    { }
    Metric(const Metric& other)
        : _metric(other._metric)
        , _tagsMetric(NULL)
    { }
    ~Metric() {
        if (_tagsMetric) {
            _metric->UndeclareMetric(_tagsMetric);
            _tagsMetric = NULL;
        }
    }

public:
    // immutable tags
    typedef std::vector<std::pair<std::string, std::string>> KVVec;
    void addTags(const KVVec& tagPairs) {
        kmonitor::MetricsTags tags;
        for (auto kvPair : tagPairs) {
            tags.AddTag(kvPair.first, kvPair.second);
        }
        addTags(tags);
    }

    void addTags(kmonitor::MetricsTags& tags) {
        assert(_metric);
        assert(!_tagsMetric);
        _tagsMetric = _metric->DeclareMetric(&tags);
    }

    void report(double value) {
        if (_tagsMetric) {
            _tagsMetric->Update(value);            
        }
    }

    // create a Metric with new tags
    MetricPtr declareMetric(kmonitor::MetricsTags& tags) {
        MetricPtr retMetric(new Metric(*this));
        retMetric->addTags(tags);
        return retMetric;
    }
    
    LightMetricPtr declareLightMetric(kmonitor::MetricsTags& tags) {
        return LightMetricPtr(new LightMetric(_metric->DeclareMetric(&tags)));
    }

public:
    // mutable tags
    void report(kmonitor::MetricsTags& tags, double value) {
        assert(_metric);
        _metric->Report(&tags, value);
    }
    void report(const kmonitor::MetricsTagsPtr& tags, double value) {
        assert(_metric);
        _metric->Report(tags, value);
    }

protected:
    std::shared_ptr<kmonitor::MutableMetric> _metric;
    kmonitor::Metric* _tagsMetric = NULL;
};

#define KMONITOR_ADAPTER_REPORT(metric, value)     \
    if (metric){                                   \
        metric->report(value);                     \
    }

#define KMONITOR_ADAPTER_REPORT_TAGS(metric, tag, value)    \
    if (metric){                                            \
        metric->report(tag, value);                         \
    }

}

