/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-03-09 11:01
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 */

#ifndef KMONITOR_CLIENT_KMONITORWORKER_H_
#define KMONITOR_CLIENT_KMONITORWORKER_H_

#include <map>
#include <string>
#include <autil/Lock.h>
#include "kmonitor/client/common/Log.h"
#include "kmonitor/client/common/Common.h"


BEGIN_KMONITOR_NAMESPACE(kmonitor);

class MetricsSystem;
class MetricsConfig;
class KMonitor;
class MetricsTags;

class KMonitorWorker {
 public:
    KMonitorWorker();
    ~KMonitorWorker();
    bool Init(const std::string& config_content);
    bool Init(const MetricsConfig& config);
    void Start();
    bool IsStarted();
    void Shutdown();
    void SetServiceName(const std::string &name);
    void AddGlobalTag(const std::string &k, const std::string &v);
    const std::string& ServiceName();
    KMonitor *GetKMonitor(const std::string &name, bool useMetricCache = true);
    void ReleaseKMonitor(const std::string &name);
    std::map<std::string, KMonitor*> *GetKMonitorMap();
    MetricsSystem* getMetricsSystem() { return system_; }
    MetricsConfig* getConfig() { return config_; }
    void registerBuildInMetrics(MetricsTags *tags, const std::string& metric_name_prefix = "");
 public:
    static void addGlobalTags(MetricsConfig &config);
 private:
    static void EscapeString(std::string& input);
 private:
    KMonitorWorker(const KMonitorWorker &);
    KMonitorWorker &operator=(const KMonitorWorker &);

 private:
    std::string name_;
    bool started_;
    MetricsSystem *system_;
    MetricsConfig *config_;
    std::map<std::string, KMonitor*> *kmonitor_map_;
    autil::ThreadMutex metric_mutex_;

 private:
    KMONITOR_LOG_DECLARE();
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_KMONITORWORKER_H_
