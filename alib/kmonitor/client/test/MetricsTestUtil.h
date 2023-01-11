/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2020-06-17
 * Author Name: dongdong.shan, wister.zwz
 * Author Email: dongdong.shan@alibaba-inc.com, wister.zwz@alibaba-inc.com
 * */

#pragma once
#include <unittest/unittest.h>
#include <kmonitor/client/MetricsReporter.h>
#include <kmonitor/client/metric/QpsMetric.h>
#include <kmonitor/client/metric/GaugeMetric.h>
#include <kmonitor/client/core/MetricsManager.h>
#include <kmonitor/client/core/MetricsMap.h>
#include <kmonitor/client/core/MetricsData.h>
#include <string>
#include <functional>

BEGIN_KMONITOR_NAMESPACE(kmonitor);

class MetricsTestUtil {
public:
    static void checkMetricWithValue(MetricsReporter *reporter,
            const std::string &name,
            const MetricsTags *tags,
            double expected)
    {
        ASSERT_NO_FATAL_FAILURE(checkMetricWithFunc(reporter, name, tags,
                        [expected] (double value) {
                            ASSERT_DOUBLE_EQ(expected, value);
                        }));
    }

    static void checkMetricWithFunc(MetricsReporter *reporter,
            const std::string &name,
            const MetricsTags *tags,
            std::function<void(double)> func) {
        ASSERT_NE(nullptr, reporter);
        auto mergedTags = reporter->_tags;
        if (tags) {
            tags->MergeTags(&mergedTags);
        }

        auto *metricsManager = reporter->_monitor->manager_;
        ASSERT_NE(nullptr, metricsManager);
        auto &metricsMap = metricsManager->sharded_metrics_[metricsManager->GetShardId(name)];
        auto dataIt = metricsMap.metric_data_map_.find(name);
        ASSERT_TRUE(dataIt != metricsMap.metric_data_map_.end());
        auto *metricsData = dataIt->second;
        ASSERT_NE(nullptr, metricsData);

        auto metricIt = metricsData->metric_map_.find(mergedTags.Hashcode());
        if (metricIt == metricsData->metric_map_.end()) {
            // special case: treat non-exist metric as value == 0
            ASSERT_NO_FATAL_FAILURE(func(0));
            return;
        }
        Metric *metric = metricIt->second.second;
        ASSERT_NE(nullptr, metric);
        auto *qpsMetric = dynamic_cast<QpsMetric*>(metric);
        if (qpsMetric) {
            ASSERT_NO_FATAL_FAILURE(func(qpsMetric->value_));
            return;
        }
        auto *gaugeMetric = dynamic_cast<GaugeMetric*>(metric);
        if (gaugeMetric) {
            ASSERT_EQ(1, gaugeMetric->count_);
            ASSERT_NO_FATAL_FAILURE(func(gaugeMetric->sum_));
            return;
        }
        ASSERT_FALSE(true) << "unsupported metric type";
    }
};

END_KMONITOR_NAMESPACE(kmonitor);
