/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-24 10:47
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 */

#ifndef KMONITOR_CLIENT_METRIC_RAWMETRIC_H_
#define KMONITOR_CLIENT_METRIC_RAWMETRIC_H_

#include <string>
#include <vector>
#include "kmonitor/client/common/Common.h"
#include "kmonitor/client/metric/Metric.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);


class RawMetric : public Metric {
 public:
    RawMetric();
    ~RawMetric();
    explicit RawMetric(const std::string &name);
    void Update(double value);
    void Snapshot(MetricsRecordBuilder* builder, bool all);

 private:
    RawMetric(const RawMetric &);
    RawMetric &operator=(const RawMetric &);

 private:
    MetricsInfoPtr info_;
    std::vector<double> values_;
 private:
    friend class RawMetricTest_TestUpdate_Test;
    friend class RawMetricTest_TestSnapshot_Test;
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_METRIC_RAWMETRIC_H_
