#ifndef __BEEPER_TYPEDEF_H
#define __BEEPER_TYPEDEF_H

#include <tr1/memory>
#include <sys/time.h>
#include <autil/TimeUtility.h>
#include <autil/legacy/jsonizable.h>
#include "beeper/common/common.h"

namespace beeper { 

enum EventLevel
{
    EL_DEFAULT = 0,
    EL_HIGH = 1,
    EL_MIDDDLE = 2,
    EL_LOW = 3,
    EL_TRACE = 4,    
    EL_UDF1 = 5,
    EL_UDF2 = 6,
    EL_UDF3 = 7,
    EL_UDF4 = 8,
    EL_UDF5 = 9,
    EL_UNKNOWN = 255,
};

inline const char* LevelToString(EventLevel level)
{
    switch (level)
    {
    case EL_DEFAULT:
        return "default";
    case EL_HIGH:
        return "high";
    case EL_MIDDDLE:
        return "middle";
    case EL_LOW:
        return "low";
    case EL_TRACE:
        return "trace";
    case EL_UDF1:
        return "udf1";
    case EL_UDF2:
        return "udf2";
    case EL_UDF3:
        return "udf3";
    case EL_UDF4:
        return "udf4";
    case EL_UDF5:
        return "udf5";
    default:
        return "unknown";
    }
}

inline EventLevel LevelFromString(const std::string& level)
{
    if (level == "default") 
    {
        return EL_DEFAULT;
    }
    if (level == "high")
    {
        return EL_HIGH;
    }
    if (level == "middle")
    {
        return EL_MIDDDLE;
    }
    if (level == "low")
    {
        return EL_LOW;       
    }
    if (level == "trace")
    {
        return EL_TRACE;
    }
    if (level == "udf1")
    {
        return EL_UDF1;
    }
    if (level == "udf2")
    {
        return EL_UDF2;
    }
    if (level == "udf3")
    {
        return EL_UDF3;
    }
    if (level == "udf4")
    {
        return EL_UDF4;
    }
    if (level == "udf5")
    {
        return EL_UDF5;
    }
    return EL_UNKNOWN;
}


////////////////////////////////////////////////////////////////////////////
class EventTags
{
public:
    typedef std::map<std::string, std::string> TagMap;
    typedef TagMap::const_iterator TagMapIter;
    
public:
    class Iterator
    {
    public:
        Iterator(const TagMap& data)
            : mData(data)
            , mIter(mData.begin())
        {}
        Iterator(const Iterator& iter)
            : mData(iter.mData)
            , mIter(iter.mIter)
        {}

    public:
        bool IsValid() const { return mIter != mData.end(); }
        const std::string& GetKey() const { return mIter->first; }
        const std::string& GetValue() const { return mIter->second; }
        void MoveToNext() { ++mIter; }
        
    private:
        const TagMap& mData;
        TagMapIter mIter;
    };
    
public:
    EventTags(const TagMap& _kvMap) : mKVMap(_kvMap) {}
    EventTags() {}
    ~EventTags() {}

public:
    void AddTag(const std::string &k, const std::string &v)
    {
        mKVMap[k] = v;
    }
    
    const std::string& FindTag(const std::string &k) const
    {
        auto it = mKVMap.find(k);
        if (it == mKVMap.end())
        {
            static std::string emptyStr;
            return emptyStr;
        }
        return it->second; 
    }
    
    void DelTag(const std::string& k) { mKVMap.erase(k); }
    
    void MergeTags(EventTags* tags) const
    {
        auto it = mKVMap.begin();
        for (; it != mKVMap.end(); ++it)
        {
            tags->AddTag(it->first, it->second);
        }
    }
    size_t Size() const { return mKVMap.size(); }
    Iterator CreateIterator() const { return Iterator(mKVMap); }
    std::string ToString() const { return autil::legacy::ToJsonString(mKVMap, true); }
    
private:
    TagMap mKVMap;
};
typedef std::tr1::shared_ptr<EventTags> EventTagsPtr;


////////////////////////////////////////////////////////////////////////////
class Event
{
public:
    Event(const std::string& collecterId, const std::string& _msg,
          const EventTagsPtr& _tags, int64_t _ts)
        : mCollectorId(collecterId)
        , mMsgData(_msg)
        , mTags(_tags)
        , mTimestamp(_ts)
    {}

    const std::string& GetCollectorId() const { return mCollectorId; }
    const std::string& GetMsg() const { return mMsgData; }
    const EventTagsPtr& GetTags() const { return mTags; }
    int64_t GetTimestamp() const { return mTimestamp; }
    std::string GetTimestampString(const std::string& format = "%Y%m%d%H%M%S") const
    {
        time_t tmp = mTimestamp / 1000000;
        struct tm cvt;
        localtime_r(&tmp, &cvt);
        char str[200];
        strftime(str, sizeof(str), format.c_str(), &cvt);
        return std::string(str);
    }
    
    std::string ToString() const
    {
        return std::string("__collector__:") + mCollectorId +
            std::string(";__message__:") + mMsgData +
            ";__timestamp__=" + GetTimestampString() +
            ";__tags__=" + ((mTags != NULL) ? mTags->ToString() : "{}");
    }

private:
    std::string mCollectorId;
    std::string mMsgData;
    EventTagsPtr mTags;  // tags object should be read only
    int64_t mTimestamp;  // us
};

typedef std::tr1::shared_ptr<Event> EventPtr;

}

#endif //__BEEPER_BEEPER_H
