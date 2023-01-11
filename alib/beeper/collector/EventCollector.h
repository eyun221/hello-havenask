#ifndef __BEEPER_EVENTCOLLECTOR_H
#define __BEEPER_EVENTCOLLECTOR_H

#include <tr1/memory>
#include <autil/Lock.h>
#include "beeper/common/common.h"
#include "beeper/common/common_type.h"

BP_DECLARE_REFERENCE_CLASS(reporter, EventReporterContainer);

BP_NAMESPACE_BEGIN(collector);

class EventCollector
{
public:
    EventCollector(const std::string& id, const EventTags& tags,
                   EventLevel defaultLevel,
                   const reporter::EventReporterContainerPtr& container);
    
    ~EventCollector();
    
public:
    void Collect(EventLevel level, const std::string& msg,
                 const EventTags& udfTags = EventTags());
    
    void Collect(const std::string& msg, const EventTags& udfTags = EventTags())
    {
        Collect(mDefaultLevel, msg, udfTags);
    }

    const std::string& GetId() const { return mId; }

    void AddTag(const std::string& key, const std::string& value);

private:
    EventTagsPtr GetCollectorTags() const;
    
private:
    reporter::EventReporterContainerPtr mReporterContainer;
    EventTagsPtr mTags;
    std::string mId;
    EventLevel mDefaultLevel;
    mutable autil::RecursiveThreadMutex mLock;
    
private:
    BP_LOG_DECLARE();
};

typedef std::tr1::shared_ptr<EventCollector> EventCollectorPtr;

BP_NAMESPACE_END(collector);

#endif //__BEEPER_EVENTCOLLECTOR_H
