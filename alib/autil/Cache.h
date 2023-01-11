#ifndef AUTIL_CACHE_H
#define AUTIL_CACHE_H

#include <vector>
#include "autil/CommonMacros.h"

AUTIL_BEGIN_NAMESPACE(autil);

template<class K, class V>
class Cache
{
public:
    virtual ~Cache(){}
    
public:
    virtual void setCacheSize(uint64_t cacheSize) = 0;
    virtual void warmUp(const std::vector<K>& keyList, const std::vector<V>& valueList) = 0;
    virtual bool invalidate(const K& key) = 0;
    virtual void invalidate(const std::vector<K>& keyList) = 0;
    virtual bool put(const K& key, const V& val) = 0;
    virtual bool get(const K& key, V& val) = 0;
    virtual bool update(const K& key, const V& newVal) = 0;
    virtual float getHitRatio() const = 0;
    virtual bool isInCache(const K& key) = 0;
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_CACHE_H
