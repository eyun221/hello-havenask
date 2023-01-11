#ifndef __BEEPER_LOGFILEEVENTRECORDER_H
#define __BEEPER_LOGFILEEVENTRECORDER_H

#include <tr1/memory>
#include <alog/Logger.h>
#include "beeper/common/common.h"
#include "beeper/recorder/EventRecorder.h"

BP_NAMESPACE_BEGIN(recorder);

class LogFileEventRecorder : public EventRecorder
{
public:
    LogFileEventRecorder(const std::string& identifier);
    ~LogFileEventRecorder();
    
public:
    bool Init(const std::map<std::string, std::string>& kvParams) override;
    void Close() override;
    bool Process(EventLevel level, const EventPtr& event) override;

private:
    bool OpenLogFile();
    
private:
    bool mUsePidSuffix;
    bool mSyncFlush;
    bool mRotateFile;
    std::string mFilePath;
    alog::Logger* mLogger;
    
private:
    BP_LOG_DECLARE();
};

typedef std::tr1::shared_ptr<LogFileEventRecorder> LogFileEventRecorderPtr;

BP_NAMESPACE_END(recorder);

#endif //__BEEPER_LOGFILEEVENTRECORDER_H
