/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2019-07-11 19:00
 * Author Name: yixuan.ly
 * Author Email: yixuan.ly@alibaba-inc.com
 */

#ifndef KMONITOR_CLIENT_METRIC_SUMMARYMETRIC_H_
#define KMONITOR_CLIENT_METRIC_SUMMARYMETRIC_H_

#include <string>
#include <vector>
#include "kmonitor/client/common/Common.h"
#include "kmonitor/client/metric/Metric.h"
#include "kmonitor/client/metric/DDSketch.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);


class SummaryMetric : public Metric {
 public:
    ~SummaryMetric();
    explicit SummaryMetric(const std::string &name);
    void Update(double value);
    void Snapshot(MetricsRecordBuilder* builder, bool all);

 private:
    SummaryMetric(const SummaryMetric &);
    SummaryMetric &operator=(const SummaryMetric &);

 private:
    MetricsInfoPtr info_;
    DDSketch* ddsketch_;
    static const double RELATIVE_ACCURACY;
    
 private:
    friend class SummaryMetricTest_TestUpdate_Test;
    friend class SummaryMetricTest_TestSnapshot_Test;
};

TYPEDEF_PTR(SummaryMetric);

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_METRIC_RAWMETRIC_H_
