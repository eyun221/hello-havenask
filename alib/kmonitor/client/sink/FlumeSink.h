/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-24 15:00
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 */

#ifndef KMONITOR_CLIENT_SINK_FLUMESINK_H_
#define KMONITOR_CLIENT_SINK_FLUMESINK_H_

#include <string>
#include <autil/CircularQueue.h>
#include "kmonitor/client/common/Log.h"
#include "kmonitor/client/common/Common.h"
#include "kmonitor/client/core/MetricsConfig.h"
#include "kmonitor/client/core/MetricsRecord.h"
#include "kmonitor/client/net/thrift/ThriftFlumeEvent.h"
#include "kmonitor/client/net/BatchFlumeEvent.h"
#include "kmonitor/client/net/BaseAgentClient.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);

class AgentClient;

class FlumeSink {
 public:
    explicit FlumeSink(MetricsConfig *config);
    explicit FlumeSink(MetricsConfig *config, BaseAgentClient* client, BaseAgentClient* remoteClient);
    ~FlumeSink();
    bool Init();
    void PutMetrics(MetricsRecord *record, BatchFlumeEventPtr events);
    void Flush(const BatchFlumeEventPtr &events);
    const std::string &Name();
    bool ReConnect();

 private:
    FlumeSink(const FlumeSink &);
    FlumeSink &operator=(const FlumeSink &);
    void enqueueEvents(const BatchFlumeEventPtr &events);
    void disconnectRemoteClient();
    void sendQueueEvents();

 private:
    bool started_;
    std::string name_;
    std::string topic_;
    MetricsConfig *config_;
    BaseAgentClient* client_;
    BaseAgentClient* remote_client_;
    int remote_send_cnt_;
    autil::CircularQueue<BatchFlumeEventPtr> queue_;
 public:
    static const int32_t SOCK_TIMEOUT_MS = 100;
    static const size_t QUEUE_CAPACITY = 100;
    static const int CLOSE_REMOTE_FREQ = 20;
    static const int REMOTE_SEND_TIMES = 3;

 private:
    KMONITOR_LOG_DECLARE();
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_SINK_FLUMESINK_H_
