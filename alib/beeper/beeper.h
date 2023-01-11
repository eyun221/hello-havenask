#ifndef __BEEPER_BEEPER_H
#define __BEEPER_BEEPER_H

#include "beeper/collector/EventCollectorAllocator.h"
#include <autil/TimeUtility.h>

static int BEEPER_MAX_MSG_LENGTH = 1024;
#define BEEPER_INIT_FROM_LOCAL_CONF_FILE(localConfFile, globalTags...)  \
    do {                                                                \
        char* envStr = getenv("DISABLE_BEEPER");                        \
        if (envStr && std::string(envStr) == "true")                    \
        {                                                               \
            beeper::collector::EventCollectorAllocator::DisableEventCollect(); \
        }                                                               \
        else                                                            \
        {                                                               \
            auto allocator = beeper::collector::EventCollectorAllocator::GetInstance(); \
            if (allocator->InitFromConfigFile(localConfFile, ##globalTags)) \
            {                                                           \
                beeper::collector::EventCollectorAllocator::EnableEventCollect(); \
            }                                                           \
        }                                                               \
    } while (0)

#define BEEPER_INIT_FROM_CONF_STRING(configStr, globalTags...)          \
    do {                                                                \
        char* envStr = getenv("DISABLE_BEEPER");                        \
        if (envStr && std::string(envStr) == "true")                    \
        {                                                               \
            beeper::collector::EventCollectorAllocator::DisableEventCollect(); \
        }                                                               \
        else                                                            \
        {                                                               \
            auto allocator = beeper::collector::EventCollectorAllocator::GetInstance(); \
            if (allocator->InitFromConfigString(configStr, ##globalTags)) \
            {                                                           \
                beeper::collector::EventCollectorAllocator::EnableEventCollect(); \
            }                                                           \
        }                                                               \
    } while (0)

#define BEEPER_ADD_GLOBAL_TAG(key, value)                               \
    do {                                                                \
        if (beeper::collector::EventCollectorAllocator::UsingEventCollect()) \
        {                                                               \
            auto allocator = beeper::collector::EventCollectorAllocator::GetInstance(); \
            allocator->AddGlobalTag(key, value);                        \
        }                                                               \
    } while (0)
    
#define DECLARE_BEEPER_COLLECTOR(id, collectorTags...)                  \
    do {                                                                \
        if (beeper::collector::EventCollectorAllocator::UsingEventCollect()) \
        {                                                               \
            auto allocator = beeper::collector::EventCollectorAllocator::GetInstance(); \
            allocator->DeclareCollector(id, ##collectorTags);           \
        }                                                               \
    } while (0)

#define REMOVE_BEEPER_COLLECTOR(id)                                     \
    do {                                                                \
        if (beeper::collector::EventCollectorAllocator::UsingEventCollect()) \
        {                                                               \
            auto allocator = beeper::collector::EventCollectorAllocator::GetInstance(); \
            allocator->RemoveCollector(id);                             \
        }                                                               \
    } while (0)

#define BEEPER_LEVEL_REPORT(id, level, msg, eventTags...)               \
    do {                                                                \
        if (beeper::collector::EventCollectorAllocator::UsingEventCollect()) \
        {                                                               \
            auto allocator = beeper::collector::EventCollectorAllocator::GetInstance(); \
            auto collector = allocator->FindCollector(id);              \
            if (collector)                                              \
            {                                                           \
                collector->Collect(level, msg, ##eventTags);            \
            }                                                           \
        }                                                               \
    } while (0)

#define BEEPER_REPORT(id, msg, eventTags...)                            \
    do {                                                                \
        if (beeper::collector::EventCollectorAllocator::UsingEventCollect()) \
        {                                                               \
            auto allocator = beeper::collector::EventCollectorAllocator::GetInstance(); \
            auto collector = allocator->FindCollector(id);              \
            if (collector)                                              \
            {                                                           \
                collector->Collect(msg, ##eventTags);                   \
            }                                                           \
        }                                                               \
    } while (0)

#define BEEPER_FORMAT_REPORT(id, eventTags, format, args...)            \
    do {                                                                \
        if (beeper::collector::EventCollectorAllocator::UsingEventCollect()) \
        {                                                               \
            auto allocator = beeper::collector::EventCollectorAllocator::GetInstance(); \
            auto collector = allocator->FindCollector(id);              \
            if (collector)                                              \
            {                                                           \
                char buffer[BEEPER_MAX_MSG_LENGTH];                     \
                snprintf(buffer, BEEPER_MAX_MSG_LENGTH, format, args);  \
                std::string msg(buffer);                                \
                collector->Collect(msg, eventTags);                     \
            }                                                           \
        }                                                               \
    } while (0)

#define BEEPER_FORMAT_REPORT_WITHOUT_TAGS(id, format, args...)          \
    do {                                                                \
        if (beeper::collector::EventCollectorAllocator::UsingEventCollect()) \
        {                                                               \
            auto allocator = beeper::collector::EventCollectorAllocator::GetInstance(); \
            auto collector = allocator->FindCollector(id);              \
            if (collector)                                              \
            {                                                           \
                char buffer[BEEPER_MAX_MSG_LENGTH];                     \
                snprintf(buffer, BEEPER_MAX_MSG_LENGTH, format, args);  \
                std::string msg(buffer);                                \
                collector->Collect(msg);                                \
            }                                                           \
        }                                                               \
    } while (0)


#define BEEPER_INTERVAL_REPORT(interval, id, msg, eventTags...)         \
    do {                                                                \
        static int64_t logTimestamp;                                    \
        int64_t now = autil::TimeUtility::currentTimeInSeconds();       \
        if (now - logTimestamp > interval) {                            \
            BEEPER_REPORT(id, msg, ##eventTags);                        \
            logTimestamp = now;                                         \
        }                                                               \
    } while (0)

#define BEEPER_FORMAT_INTERVAL_REPORT(interval, id, eventTags, format, args...) \
    do {                                                                \
        static int64_t logTimestamp;                                    \
        int64_t now = autil::TimeUtility::currentTimeInSeconds();       \
        if (now - logTimestamp > interval) {                            \
            BEEPER_FORMAT_REPORT(id, eventTags, format, args);          \
            logTimestamp = now;                                         \
        }                                                               \
    } while (0)


#define BEEPER_INTERVAL_LEVEL_REPORT(interval, id, level, msg, eventTags...) \
    do {                                                                \
        static int64_t logTimestamp;                                    \
        int64_t now = autil::TimeUtility::currentTimeInSeconds();       \
        if (now - logTimestamp > interval) {                            \
            BEEPER_LEVEL_REPORT(id, level, msg, ##eventTags);           \
            logTimestamp = now;                                         \
        }                                                               \
    } while (0)


#define BEEPER_CLOSE()                                                  \
    do {                                                                \
        beeper::collector::EventCollectorAllocator::GetInstance()->Reset(); \
    } while (0)


#endif //__BEEPER_BEEPER_H
