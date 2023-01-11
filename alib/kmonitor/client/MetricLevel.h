/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-08-03 09:52
 * Author Name: beifei
 * Author Email: beifei@taobao.com
 */

#ifndef KMONITOR_CLIENT_METRICLEVEL_H_
#define KMONITOR_CLIENT_METRICLEVEL_H_

#include <set>
#include <map>
#include <string>
#include "kmonitor/client/common/Common.h"
BEGIN_KMONITOR_NAMESPACE(kmonitor);

// kmonitor server store metrics for 20S,1m,10m,60m, sdk should align for it

enum MetricLevel {
    FATAL = 1,
    CRITICAL = 2,
    MAJOR = 5,
    NORMAL = 10,
    MINOR = 30,
    TRIVIAL = 60
};

class MetricLevelManager {
 public:
    MetricLevelManager();
    ~MetricLevelManager();

 public:
    void AddMetricLevel(const std::string& metric_name, MetricLevel level);
    void RemoveMetricLevel(const std::string& metric_name);
    std::set<std::string> GetMetric(MetricLevel level);
    static std::set<MetricLevel> GetLevel(int second);

 private:
    MetricLevelManager(const MetricLevelManager &);
    MetricLevelManager& operator=(const MetricLevelManager &);

 private:
    std::map<std::string, MetricLevel> metric_map_;
    std::map<MetricLevel, std::set<std::string> > level_map_;
};



END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_METRICLEVEL_H_
