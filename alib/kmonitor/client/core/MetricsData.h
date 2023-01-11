/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-27 10:04
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 * */

#ifndef KMONITOR_CLIENT_CORE_METRICSDATA_H_
#define KMONITOR_CLIENT_CORE_METRICSDATA_H_

#include <string>
#include <map>
#include <queue>
#include <vector>
#include <utility>
#include "autil/Lock.h"
#include "kmonitor/client/common/Common.h"
#include "kmonitor/client/common/Log.h"
#include "kmonitor/client/MetricType.h"
#include "kmonitor/client/core/MetricsTags.h"


BEGIN_KMONITOR_NAMESPACE(kmonitor);

class Metric;
class MetricsCollector;
class MetricsTagsManager;
class MetricsCache;

class MetricsData {
 public:
    MetricsData(MetricsTagsManager *tags_manager = NULL, MetricsCache* metricsCache = NULL);
    ~MetricsData();
    MetricsData(const std::string &name, MetricType type, int statistics_type,
                MetricsTagsManager *tags_manager = NULL, MetricsCache* metricsCache = NULL);
    int Size();
    void Snapshot(MetricsCollector* collector, bool all, int64_t curTime);
    Metric* GetMetric(const MetricsTagsPtr& tags);
    Metric* GetMetric(const MetricsTags *tags);
    MetricType GetMetricType();
    int GetStatisticsType();
    void Unregister();

 private:
    MetricsData(const MetricsData &);
    MetricsData &operator=(const MetricsData &);

 private:
    std::string metric_name_;
    MetricType type_ = GAUGE;
    int statistics_type_ = 0;
    bool registered_ = true;
    //metrics_tags_vec_ and metric_map_ should in one map
    //std::vector<MetricsTags*> metrics_tags_vec_;
    //std::map<int64_t, Metric*> metric_map_;
    std::map<int64_t, std::pair<MetricsTagsPtr, Metric*> > metric_map_;
    std::queue<Metric*> metric_pool_; //回收metric
    autil::ReadWriteLock metric_rwlock_;
    MetricsTagsManager *tags_manager_ = nullptr;
    MetricsCache* metricsCache_ = nullptr;  // for release cache for metric

 private:
    KMONITOR_LOG_DECLARE();
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_CORE_METRICSDATA_H_
