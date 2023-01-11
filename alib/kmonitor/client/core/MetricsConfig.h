/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-27 16:13
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 * */

#ifndef KMONITOR_CLIENT_CORE_METRICSCONFIG_H_
#define KMONITOR_CLIENT_CORE_METRICSCONFIG_H_

#include <map>
#include <vector>
#include <string>
#include <autil/legacy/jsonizable.h>
#include "kmonitor/client/common/Common.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);

class MetricsTags;

class MetricsConfig : public autil::legacy::Jsonizable {
 public:
    MetricsConfig();
    ~MetricsConfig();

    void Jsonize(autil::legacy::Jsonizable::JsonWrapper& json);

    void AddGlobalTag(const std::string &k, const std::string &v);
    const MetricsTags* global_tags() const;
    bool inited() const;
    void set_inited(bool flag);
    const std::string& service_name() const;
    void set_service_name(const std::string& service_name);
    const std::string& tenant_name() const;
    void set_tenant_name(const std::string& tenant_name);
    const std::string& sink_address() const;
    void set_sink_address(const std::string& sink_address);
    const std::string& remote_sink_address() const;
    void set_remote_sink_address(const std::string& remote_sink_address);
    bool enable_remote_sink() const;
    void set_enable_remote_sink(bool enable_remote_sink);

 public:
    MetricsConfig &operator=(const MetricsConfig &config);

 private:
    MetricsConfig(const MetricsConfig &);

 private:
    bool inited_;
    std::string tenant_name_;
    std::string service_name_;
    bool enable_system_;
    int sink_period_;
    int sink_queue_capacity_;
    std::string sink_address_;
    std::string remote_sink_address_;
    bool enable_remote_sink_;
    MetricsTags *global_tags_;
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_CORE_METRICSCONFIG_H_
