/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-24 15:44
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 */

#ifndef KMONITOR_CLIENT_CORE_METRICSRECORDBUILDER_H_
#define KMONITOR_CLIENT_CORE_METRICSRECORDBUILDER_H_

#include <map>
#include <string>
#include <vector>
#include "kmonitor/client/common/Common.h"
#include "kmonitor/client/core/MetricsValue.h"
#include "kmonitor/client/core/MetricsTags.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);


class MetricsRecord;

class MetricsRecordBuilder {
 public:
    explicit MetricsRecordBuilder(const std::string &name, int64_t curTime);
    ~MetricsRecordBuilder();
    MetricsRecordBuilder *AddMetric(const MetricsInfoPtr &info, double value);
    MetricsRecordBuilder *AddMetric(const MetricsInfoPtr &info, std::string values);
    MetricsRecordBuilder *AddTags(const MetricsTagsPtr &tags);
    MetricsRecord *GetRecord() const;

 private:
    MetricsRecordBuilder(const MetricsRecordBuilder &);
    MetricsRecordBuilder &operator=(const MetricsRecordBuilder &);

 private:
    int64_t timestamp_;
    MetricsInfoPtr metrics_info_;
    std::vector<MetricsValue*> metrics_value_;
    MetricsTagsPtr metrics_tags_;
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_CORE_METRICSRECORDBUILDER_H_
