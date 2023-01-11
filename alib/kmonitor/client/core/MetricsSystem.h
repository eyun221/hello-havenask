/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-27 10:06
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 * */

#ifndef KMONITOR_CLIENT_CORE_METRICSSYSTEM_H_
#define KMONITOR_CLIENT_CORE_METRICSSYSTEM_H_

#include <set>
#include <string>
#include <vector>
#include <map>
#include <autil/LoopThread.h>
#include "kmonitor/client/MetricLevel.h"
#include "kmonitor/client/common/Common.h"
#include "kmonitor/client/common/Log.h"
#include "kmonitor/client/sink/FlumeSink.h"
#include <autil/Lock.h>

BEGIN_KMONITOR_NAMESPACE(kmonitor);

class MetricsConfig;
class MetricsSource;
class MetricsSink;
class MetricsConfig;
class MetricsRecord;
class MetricsCollector;

class MetricsSystem {
 public:
    explicit MetricsSystem(MetricsConfig *config);
    ~MetricsSystem();

    void Start();
    void Stop();
    bool Started();

 private:
    MetricsSystem(const MetricsSystem &);
    MetricsSystem &operator=(const MetricsSystem &);
    std::vector<MetricsRecord*>* SampleMetrics(const std::set<MetricLevel>& levels);
    void PublishMetrics(std::vector<MetricsRecord *> *records, bool immediate);
    void StartTimer();
    void OnTimerEvent();

 public:
    void AddSource(MetricsSource *source);
    void DelSource(const std::string &sourceName);

 private:
    int ticker_;
    int period_;
    std::string prefix_;
    bool started_;
    MetricsConfig *config_;
    MetricsCollector *collector_;
    mutable autil::ReadWriteLock source_lock_;
    std::map<std::string, MetricsSource*> source_map_;
    FlumeSink* sink_;
    autil::LoopThreadPtr timer_thread_ptr_;

 private:
    KMONITOR_LOG_DECLARE();
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_CORE_METRICSSYSTEM_H_
