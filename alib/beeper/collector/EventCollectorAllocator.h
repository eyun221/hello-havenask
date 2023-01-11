#ifndef __BEEPER_EVENT_COLLECTOR_ALLOCATOR_H
#define __BEEPER_EVENT_COLLECTOR_ALLOCATOR_H

#include <tr1/memory>
#include <autil/Lock.h>
#include <autil/Log.h>
#include <unordered_map>
#include "beeper/common/common.h"
#include "beeper/collector/EventCollector.h"

BP_DECLARE_REFERENCE_CLASS(reporter, EventReporterContainer);
BP_DECLARE_REFERENCE_CLASS(config, CollectorParamConfig);

BP_NAMESPACE_BEGIN(collector);

class EventCollectorAllocator
{
public:
    class Creator
    {
    public:
        static EventCollectorAllocator* Create()
        {
            EventCollectorAllocator* ptr = new EventCollectorAllocator();
            static std::tr1::shared_ptr<EventCollectorAllocator> destroyer(ptr);
            return ptr;
        }
    };

public:
    EventCollectorAllocator();
    ~EventCollectorAllocator();

private:
    EventCollectorAllocator(const EventCollectorAllocator &);
    EventCollectorAllocator& operator = (const EventCollectorAllocator &);
    
public:
    static EventCollectorAllocator* GetInstance()
    {
        if (unlikely(!mPtr))
        {
            autil::ScopedLock sl(mLock);
            if (!mPtr)
            {
                mPtr = Creator::Create();
            }
        }
        return const_cast<EventCollectorAllocator*>(mPtr);
    }

    static bool UsingEventCollect() { return mEnableEventCollect; }
    static void EnableEventCollect() { mEnableEventCollect = true; }
    static void DisableEventCollect() { mEnableEventCollect = false; }

public:
    bool InitFromConfigFile(const std::string& filePath,
                            const EventTags& globalTags = EventTags());
    
    bool InitFromConfigString(const std::string& configStr,
                              const EventTags& globalTags = EventTags());
    
    EventCollectorPtr DeclareCollector(const std::string& identifier,
            const EventTags& collectorTags = EventTags());
    
    EventCollectorPtr FindCollector(const std::string& identifier) const;
    void RemoveCollector(const std::string& identifier);
    void Reset();

    void AddGlobalTag(const std::string& key, const std::string& value);

private:
    void InitDefaultGlobalTags();
    static const char* getenv(const char* envName, const char* defaultValue);
    static bool LoadFileContent(const std::string& filePath, std::string& fileContent);
    
private:
    reporter::EventReporterContainerPtr mReporterContainer;
    EventTagsPtr mGlobalTags;
    std::unordered_map<std::string, EventCollectorPtr> mCollectorMap;
    std::unordered_map<std::string, config::CollectorParamConfigPtr> mCollectorParams;
    mutable autil::RecursiveThreadMutex mCollectorLock;
    
    static bool mEnableEventCollect;
    static EventCollectorAllocator* mPtr;
    static autil::RecursiveThreadMutex mLock;

private:
    friend class EventCollectorAllocatorTest;
    BP_LOG_DECLARE();
};

typedef std::tr1::shared_ptr<EventCollectorAllocator> EventCollectorAllocatorPtr;

BP_NAMESPACE_END(collector);

#endif //__BEEPER_EVENT_COLLECTOR_ALLOCATOR_H
