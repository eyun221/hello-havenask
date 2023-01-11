/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-24 10:47
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 */

#ifndef KMONITOR_CLIENT_METRIC_STATUS_H_
#define KMONITOR_CLIENT_METRIC_STATUS_H_

#include <string>
#include "kmonitor/client/common/Common.h"
#include "kmonitor/client/metric/Metric.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);

class MetricsInfo;
class MetricsRecordBuilder;

class StatusMetric : public Metric {
 public:
    StatusMetric();
    ~StatusMetric();
    explicit StatusMetric(const std::string &name);
    void Update(double value);
    void Snapshot(MetricsRecordBuilder* builder, bool all);

 private:
    StatusMetric(const StatusMetric &);
    StatusMetric &operator=(const StatusMetric &);

 private:
    MetricsInfoPtr info_;
    double value_;
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_METRIC_STATUS_H_
