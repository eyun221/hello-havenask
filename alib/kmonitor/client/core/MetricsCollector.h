/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-27 11:00
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 * */

#ifndef KMONITOR_CLIENT_CORE_METRICSCOLLECTOR_H_
#define KMONITOR_CLIENT_CORE_METRICSCOLLECTOR_H_

#include <string>
#include <vector>
#include "kmonitor/client/common/Common.h"


BEGIN_KMONITOR_NAMESPACE(kmonitor);

class MetricsRecord;
class MetricsRecordBuilder;

class MetricsCollector {
 public:
    MetricsCollector();
    ~MetricsCollector();
    
    MetricsRecordBuilder *AddRecord(const std::string &name, int64_t curTime);

    void Clear();
    std::vector<MetricsRecord*>* GetRecords();

 private:
    MetricsCollector(const MetricsCollector &);
    MetricsCollector &operator=(const MetricsCollector &);

 private:
    std::vector<MetricsRecordBuilder*> record_builders_;
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_CORE_METRICSCOLLECTOR_H_
