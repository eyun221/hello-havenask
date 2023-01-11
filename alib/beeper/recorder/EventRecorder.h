#ifndef __BEEPER_EVENTRECORDER_H
#define __BEEPER_EVENTRECORDER_H

#include <tr1/memory>
#include <autil/Lock.h>
#include <stdlib.h>
#include "beeper/common/common.h"
#include "beeper/common/common_type.h"

BP_NAMESPACE_BEGIN(recorder);

class EventRecorder
{
public:
    EventRecorder(const std::string& identifier);
    virtual ~EventRecorder();
    
public:
    virtual bool Init(const std::map<std::string, std::string>& kvParams) = 0;
    virtual void Close() = 0;

    /* Attention: Process & BatchProcess interface should be multi-thread safe */
    virtual bool Process(EventLevel level, const EventPtr& event) = 0;
    virtual bool BatchProcess(EventLevel level, const std::vector<EventPtr>& events);

public:
    const std::string& GetIdentifier() const { return mIdentifier; }
    
    void SetRetryInterval(int64_t retryInterval) { mRetryInterval = retryInterval; }
    int64_t GetRetryInterval() const { return mRetryInterval; }

    void SetMaxRetryTimes(uint32_t retryTimes) { mMaxRetryTimes = retryTimes; }
    uint32_t GetMaxRetryTimes() const { return mMaxRetryTimes; }

    void EnableBatchProcess() { mSupportBatchMode = true; }
    bool SupportBatchProcessEvent() const { return mSupportBatchMode; }

    static void WriteDropEventLog(const std::string& recorderId,
                                  EventLevel level, const EventPtr& event);
    
protected:
    std::string mIdentifier;
    int64_t mRetryInterval;
    uint32_t mMaxRetryTimes;
    bool mSupportBatchMode;

    class DropEventLogger
    {
    public:
        DropEventLogger();
        ~DropEventLogger();
    public:
        void Write(const std::string& str);
        std::string GetLogFileName() const;
    private:
        alog::Logger* mLogger;
        autil::ThreadMutex mLock;
    };
    static DropEventLogger mDropLogger;

private:
    BP_LOG_DECLARE();
};

typedef std::tr1::shared_ptr<EventRecorder> EventRecorderPtr;

////////////////////////////////////////////////////////////////////////////////////

typedef EventRecorder* (*CreateEventRecorderFunc)(const std::string& identifier);
typedef void (*DestroyEventRecorderFunc)(EventRecorder*);


BP_NAMESPACE_END(recorder);

#endif //__BEEPER_EVENTRECORDER_H
