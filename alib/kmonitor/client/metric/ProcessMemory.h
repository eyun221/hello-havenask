/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2019-07-24 10:47
 * Author Name: yixuan.ly
 * Author Email: yixuan.ly@alibaba-inc.com
 */

#ifndef KMONITOR_CLIENT_METRIC_PROCESSMEMORY_H
#define KMONITOR_CLIENT_METRIC_PROCESSMEMORY_H

#include "kmonitor/client/core/MetricsCollector.h"
#include "kmonitor/client/common/Common.h"
#include <sstream>

BEGIN_KMONITOR_NAMESPACE(kmonitor);

class ProcessMemory
{
public:
    ProcessMemory();
    ~ProcessMemory();
private:
    ProcessMemory(const ProcessMemory &);
    ProcessMemory& operator = (const ProcessMemory &);
public:
    void Snapshot(MetricsCollector *collector, int64_t curTime);
    void update();
    uint64_t getMemSize() {return _memSize;}
    uint64_t getMemRss() {return _memRss;}
    uint64_t getMemSwap() {return _memSwap;}
    uint64_t getMemUsed() {return _memRss + _memSwap;}
    double getMemRatio() {
        double ratio = 0;
        if (_machineMemSize != 0) {
            ratio = 100.0 * getMemUsed() / _machineMemSize;
        }
        return ratio;
    }
private:
    void parseProcessMemStatus(std::istream &in);
    void parseMachineMemSize(std::istream &in);

private:
    // 单位都是KB
    uint64_t _machineMemSize;
    uint64_t _memSize;
    uint64_t _memRss;
    uint64_t _memSwap;
};

TYPEDEF_PTR(ProcessMemory);

END_KMONITOR_NAMESPACE(kmonitor);

#endif //KMONITOR_CLIENT_METRIC_PROCESSMEMORY_H
