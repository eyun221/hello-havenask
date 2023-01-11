#ifndef AUTIL_LRUCACHE_H
#define AUTIL_LRUCACHE_H

#include "autil/CommonMacros.h"
#include "autil/StringUtil.h"
#include "autil/SynchronizedHashTable.h"
#include "autil/SynchronizedList.h"
#include "autil/Cache.h"
#include "autil/Log.h"

AUTIL_BEGIN_NAMESPACE(autil);

template<class K, class V>
struct CacheValue
{
    V value;
    ListNode<K>* iter;
    CacheValue():iter(0) {}
    CacheValue(const V& v, ListNode<K>* iterator = 0)
        :value(v), iter(iterator)
    {
    }
};

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
class LruCache: public Cache<K, V>
{
public:
    LruCache(int64_t memSize,
             GetSizeCallBack getSizeCallBack = GetSizeCallBack(),
             UpdateCallBack updateCallBack = UpdateCallBack());
    /*override*/ ~LruCache();
    
public:
    /*override*/ void warmUp(const std::vector<K>& keyList, const std::vector<V>& valueList);
    /*override*/ void invalidate(const std::vector<K>& keyList);
    /*override*/ bool invalidate(const K& key);

    /*override*/ bool put(const K& key, const V& val);
    /*override*/ bool get(const K& key, V& val);
    /*override*/ bool update(const K& key, const V& newVal);
    /*override*/ bool isInCache(const K& key);

    int64_t getCacheSizeUsed() const { return _memSizeUsed; }
    int64_t getCacheSize() const { return _memSize; }
    uint64_t getKeyCount() const { return _accessQueue.getSize(); }

    /*override*/ void setCacheSize(uint64_t memSize) { _memSize = memSize; }

    GetSizeCallBack& getGetSizeCallBack()
    {
        return _getSizeCallBack;
    }

    UpdateCallBack& getUpdateCallBack()
    {
        return _updateCallBack;
    }

    int64_t getTotalQueryTimes()
    {
        return _totalQueryTimes;
    }

    int64_t getHitQueryTimes()
    {
        return _hitQueryTimes;
    }

    /* override */
    float getHitRatio() const
    {
        if (_totalQueryTimes != 0)
        {
            return (float)_hitQueryTimes / _totalQueryTimes;
        }
        else
        {
            return 0.0f;
        }
    }

    void resetHitStatistics()
    {
        _totalQueryTimes = 0;
        _hitQueryTimes = 0;
    }

private:
    bool replacement(int64_t valSize);

private:
    GetSizeCallBack _getSizeCallBack;
    UpdateCallBack _updateCallBack;

    int64_t _memSize;
    int64_t _memSizeUsed;
    RecursiveThreadMutex _mutexMemSizeUsed;

    SynchronizedHashTable<K, CacheValue<K, V> > _hashTable;

    int64_t _totalQueryTimes;
    int64_t _hitQueryTimes;

    SynchronizedList<K> _accessQueue;

private:
    static alog::Logger *_logger;
};

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
LruCache<K, V, GetSizeCallBack, UpdateCallBack>::LruCache(
        int64_t memSize, GetSizeCallBack getSizeCallBack,
        UpdateCallBack updateCallBack) 
    : _getSizeCallBack(getSizeCallBack)
    , _updateCallBack(updateCallBack)
    , _memSize(memSize)
    , _memSizeUsed(0)
    , _totalQueryTimes(0)
    , _hitQueryTimes(0)
{
}

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
LruCache<K, V, GetSizeCallBack, UpdateCallBack>::~LruCache() 
{
}

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
bool LruCache<K, V, GetSizeCallBack, UpdateCallBack>::replacement(int64_t valSize)
{
    AUTIL_LOG(TRACE1, "enter replacement, _memSizeUsed=%ld, valSize=%ld", _memSizeUsed, valSize); 
    if (_memSize < valSize)
    {
        AUTIL_LOG(WARN, "valSize exceeds the memory limit, _memSize=%ld", _memSize);
        return false;
    }

    while (valSize + _memSizeUsed > _memSize)
    {
        K nodeValue;
        bool exist = _accessQueue.getFront(nodeValue);
        if (!exist)
        {
            AUTIL_LOG(WARN, "Get Front From Access Queue Fail!");
            return false;
        }

        CacheValue<K, V> retVal;
        ReadWriteLock* pLock;
        HashEntry<K, CacheValue<K, V> >* entry = _hashTable.getHashEntry(nodeValue, pLock);
        pLock->wrlock();
        exist = entry->deleteGet(nodeValue, retVal);
        if (exist)
        {
            {
                ScopedLock l(_mutexMemSizeUsed);
                _memSizeUsed -= _getSizeCallBack(retVal.value);
            }
            ListNode<K>* iter = retVal.iter;
            _accessQueue.erase(iter);
            AUTIL_LOG(TRACE1, "delete succeeded, key=%s", StringUtil::toString(nodeValue).c_str());
        }
        else
        {
            AUTIL_LOG(TRACE1, "delete failed, key=%s", StringUtil::toString(nodeValue).c_str());
        }
        pLock->unlock();
    }
    AUTIL_LOG(TRACE1, "leave replacement, _memSizeUsed=%ld", _memSizeUsed); 
    return true;
}

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
bool LruCache<K, V, GetSizeCallBack, UpdateCallBack>::put(const K& key, const V& val)
{
    int64_t valSize = _getSizeCallBack(val);
    if (valSize + _memSizeUsed > _memSize)
    {
        bool succ = replacement(valSize);
        if (!succ)
        {
            AUTIL_LOG(WARN, "put failed, because of replacement _memSizeUsed=%ld", _memSizeUsed);
            return false;
        }
    }

    ReadWriteLock* pLock;
    HashEntry<K, CacheValue<K, V> >* entry = _hashTable.getHashEntry(key, pLock);
    pLock->wrlock();

    // if key already exists in hash table and access queue,
    // it shouldn't be inserted again, but remove old node first

    CacheValue<K, V> cv(val);
    CacheValue<K, V> oldVal;
    CacheValue<K, V>* pretVal;
    bool firstTime = entry->putGet(key, cv, oldVal, pretVal); // FIXME: if overwrite called,
                                                          // memory size used need re-calculate
    if (!firstTime)
    {
        _accessQueue.moveToBack(oldVal.iter);
        pretVal->iter = oldVal.iter;
        assert(pretVal->iter);
        AUTIL_LOG(TRACE1, "overwrite existing key, key=%s", StringUtil::toString(key).c_str()); 

        int64_t oldSize = _getSizeCallBack(oldVal.value);
        ScopedLock l(_mutexMemSizeUsed);
        _memSizeUsed += valSize - oldSize;
    }
    else
    {
        pretVal->iter = _accessQueue.pushBack(key);
        ScopedLock l(_mutexMemSizeUsed);
        _memSizeUsed += valSize;
    }

    pLock->unlock();

    //std::cout << "Put to cache: key: " << key << std::endl;

    AUTIL_LOG(TRACE1, "put succeeded, key=%s, _memSizeUsed=%ld", 
            StringUtil::toString(key).c_str(), _memSizeUsed);
    return true;
}

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
bool LruCache<K, V, GetSizeCallBack, UpdateCallBack>::get(const K& key, V& val)
{
    AUTIL_LOG(TRACE1, "enter get, key=%s", StringUtil::toString(key).c_str());
    ReadWriteLock* pLock;
    HashEntry<K, CacheValue<K, V> >* entry = _hashTable.getHashEntry(key, pLock);
    pLock->rdlock();
    
    CacheValue<K, V> cacheVal;
    bool exist = entry->get(key, cacheVal);
    if (exist)
    {
        _accessQueue.moveToBack(cacheVal.iter);
        val = cacheVal.value;
        AUTIL_LOG(TRACE1, "value got, key=%s, valueSize=%ld", 
               StringUtil::toString(key).c_str(), (int64_t)_getSizeCallBack(val));
        _hitQueryTimes ++;
    }
    else
    {
        AUTIL_LOG(TRACE1, "key not exist, key=%s", StringUtil::toString(key).c_str());
    }
    pLock->unlock();
    _totalQueryTimes ++;
    AUTIL_LOG(TRACE1, "leave get");
    return exist;
}

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
bool LruCache<K, V, GetSizeCallBack, UpdateCallBack>::isInCache(const K& key)
{
    ReadWriteLock* pLock;
    HashEntry<K, CacheValue<K, V> >* entry = _hashTable.getHashEntry(key, pLock);
    pLock->rdlock();
    
    CacheValue<K, V> cacheVal;
    bool ret = entry->get(key, cacheVal);
    pLock->unlock();
    return ret;
}

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
bool LruCache<K, V, GetSizeCallBack, UpdateCallBack>::update(
        const K& key, const V& newVal)
{
    return put(key, newVal);
}

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
bool LruCache<K, V, GetSizeCallBack, UpdateCallBack>::invalidate(const K& key)
{
    ReadWriteLock* pLock;
    HashEntry<K, CacheValue<K, V> >* entry = _hashTable.getHashEntry(key, pLock);
    pLock->wrlock();

    CacheValue<K, V> cacheVal;
    bool exist = entry->deleteGet(key, cacheVal);
    if (exist)
    {
        {
            ScopedLock l(_mutexMemSizeUsed);
            _memSizeUsed -= _getSizeCallBack(cacheVal.value);
        }
        ListNode<K>* iter = cacheVal.iter;
        _accessQueue.erase(iter);
        AUTIL_LOG(TRACE1, "invalidated key, key=%s", StringUtil::toString(key).c_str());
    }

    pLock->unlock();
    AUTIL_LOG(TRACE1, "not invalidated key, key=%s", StringUtil::toString(key).c_str());
    return exist;

}

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
void LruCache<K, V, GetSizeCallBack, UpdateCallBack>::invalidate(const std::vector<K>& keyList)
{
    typename std::vector<K>::const_iterator iter;
    for (iter = keyList.begin(); iter != keyList.end(); ++iter)
    {
        invalidate(*iter);
    }
}

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
void LruCache<K, V, GetSizeCallBack, UpdateCallBack>::warmUp(
        const std::vector<K>& keyList, const std::vector<V>& valueList)
{
    typename std::vector<K>::const_iterator iterK;
    typename std::vector<V>::const_iterator iterV;
    for (iterK = keyList.begin(), iterV = valueList.begin();
         iterK != keyList.end() && iterV != valueList.end();
         ++iterK, ++iterV)
    {
        const K& key = *iterK;
        const V& val = *iterV;
        V retVal;
        bool exist = get(key, retVal);
        if (!exist)
        {
            put(key, val);
        }
    }
}

template<class K, class V, class GetSizeCallBack, class UpdateCallBack>
alog::Logger *LruCache<K, V, GetSizeCallBack, UpdateCallBack>::_logger
= alog::Logger::getLogger("autil.LruCache");

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_LRUCACHE_H
