/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-24 15:44
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 */

#ifndef KMONITOR_CLIENT_CORE_METRICSRECORD_H_
#define KMONITOR_CLIENT_CORE_METRICSRECORD_H_

#include <map>
#include <string>
#include <vector>
#include "kmonitor/client/common/Common.h"
#include "kmonitor/client/core/MetricsValue.h"
#include "kmonitor/client/core/MetricsTags.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);

class MetricsRecord {
 public:
    MetricsRecord(MetricsInfoPtr info,
                  int64_t timestamp,
                  const MetricsTagsPtr& tags,
                  const std::vector<MetricsValue *> *metrics);
    ~MetricsRecord();

    int64_t Timestamp() const {
        return timestamp_;
    }
    const std::string& Name() const;
    const MetricsTagsPtr& Tags() const {
        return tags_;
    }
    const std::vector<MetricsValue*> *Values() const {
        return values_;
    }

 private:
    MetricsRecord(const MetricsRecord &);
    MetricsRecord &operator=(const MetricsRecord &);

 private:
    MetricsInfoPtr info_;
    int64_t timestamp_;
    MetricsTagsPtr tags_;
    const std::vector<MetricsValue*> *values_;
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_CORE_METRICSRECORD_H_
