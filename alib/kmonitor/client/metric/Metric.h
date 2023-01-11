/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-27 11:26
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 * */

#ifndef KMONITOR_CLIENT_METRIC_METRIC_H_
#define KMONITOR_CLIENT_METRIC_METRIC_H_

#include "autil/Lock.h"
#include "kmonitor/client/core/MetricsRecordBuilder.h"
#include "kmonitor/client/common/Common.h"

#define MAX_UNTOUCH_NUM 20
BEGIN_KMONITOR_NAMESPACE(kmonitor);

class Metric {
 public:
    Metric();
    virtual ~Metric();
    virtual void Update(double value) = 0;
    virtual void Snapshot(MetricsRecordBuilder* builder, bool all) = 0;

    Metric(const Metric &);

    bool canRecycle() {
        autil::ScopedLock lock(metric_mutex_);
        if (auto_recycle_) {
            //连续MAX_UNTOUCH_NUM个Snapshot周期都没有被update过，就认为可回收了
            if (untouch_num_ > MAX_UNTOUCH_NUM) {
                return true;
            }
        } else { //引用计数方式不能内部自动回收
            if (ref_cnt_ <= 0) {
                return true;
            }
        }
            
        return false;
    }

    //called is locked by metric_mutex_
    void Touch() {
        untouch_num_ = 0;
    }

    void Untouch() {
        ++untouch_num_;
    }

    void Acquire() {
        auto_recycle_ = false;
        autil::ScopedLock lock(metric_mutex_);
        ++ref_cnt_;
    }

    void Release() {
        autil::ScopedLock lock(metric_mutex_);
        --ref_cnt_;
    }

 protected:
    void SetChanged() {
        changed_ = true;
    }

    void ClearChanged() {
        changed_ = false;
    }

    bool Changed() {
        return changed_;
    }

 private:
    Metric &operator=(const Metric &);

 protected:
    bool changed_;
    bool auto_recycle_;
    uint32_t untouch_num_;
    int32_t ref_cnt_;
    autil::ThreadMutex metric_mutex_;

 public:
    static const std::string HEADER_IP;
    static const std::string HEADER_TENANT;
    static const std::string HEADER_FORMAT;
};

TYPEDEF_PTR(Metric);

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_METRIC_METRIC_H_
