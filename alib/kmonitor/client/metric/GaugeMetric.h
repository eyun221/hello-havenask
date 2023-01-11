/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-24 10:47
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 */

#ifndef KMONITOR_CLIENT_METRIC_GAUGEMETRIC_H_
#define KMONITOR_CLIENT_METRIC_GAUGEMETRIC_H_

#include <vector>
#include <string>
#include "kmonitor/client/common/Common.h"
#include "kmonitor/client/metric/Metric.h"
#include "kmonitor/client/metric/GaugeType.h"
#include "kmonitor/client/metric/SummaryMetric.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);

class SlidingWindowReservoir;
class MinMaxCalculator;
class MetricsRecordBuilder;

class GaugeMetric : public Metric {
 public:
    GaugeMetric();
    ~GaugeMetric();
    GaugeMetric(const std::string &name, int statistics_type);
    void Update(double value);
    void Snapshot(MetricsRecordBuilder* builder, bool all);

 private:
    GaugeMetric(const GaugeMetric &);
    GaugeMetric &operator=(const GaugeMetric &);
    double GetValue(GaugeType type);
    double GetMax();
    double GetMin();
    double GetAVG();
    double GetP99();
    double GetP999();


 private:
    double sum_;
    int count_;
    int statistics_type_;
    MinMaxCalculator *calculator_;
    SlidingWindowReservoir *reservoir_;
    SummaryMetric *summaryMetric_;
    MetricsInfoPtr avg_info_;
    MetricsInfoPtr max_info_;
    MetricsInfoPtr min_info_;
    MetricsInfoPtr p95_info_;
    MetricsInfoPtr p99_info_;
    MetricsInfoPtr p999_info_;
    MetricsInfoPtr sum_info_;
    MetricsInfoPtr count_info_;
    MetricsInfoPtr p75_info_;
};

TYPEDEF_PTR(GaugeMetric);

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_METRIC_GAUGEMETRIC_H_
