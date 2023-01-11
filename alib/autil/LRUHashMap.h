#ifndef AUTIL_LRUHASHMAP_H
#define AUTIL_LRUHASHMAP_H

#include "autil/CommonMacros.h"
#include "autil/LRUList.h"

AUTIL_BEGIN_NAMESPACE(autil);

class LRUHashMap
{
    typedef HashListNode* HashEntry;
public:
    LRUHashMap();
    ~LRUHashMap();
private:
    LRUHashMap(const LRUHashMap &);
    LRUHashMap& operator = (const LRUHashMap &);
public:
    bool init(uint32_t entryCount, uint32_t lockCount);
   
    inline bool tryDelete(HashListNode* node);//only delete the node of which reference count is zero
    inline HashListNode* findAndIncRef(uint64_t key);
    inline bool findAndInsert(HashListNode* node);
    
    inline uint64_t size() const;
private:
    inline HashListNode* findLockFree(uint32_t entryOffset, uint64_t key) const;
    inline bool deleteEntry(uint32_t entryOffset, HashListNode* node) ;
    size_t adjustBucketNum(size_t n) const;
private:
    ReadWriteLock *_entryLock;
    HashEntry *_entry;
    uint32_t _lockCount;
    uint32_t _entryCount;

    uint64_t _elemetCount;
private:
    friend class LRUHashMapTest;
};

inline bool LRUHashMap::tryDelete(HashListNode* node) {
    assert( _entryCount >0 && _lockCount > 0
           && NULL != _entryLock && NULL != _entry);

    if (NULL == node) {
        return false;
    }
    uint32_t entryOffset = node->getKey() % _entryCount;
    uint32_t lockOffset = entryOffset % _lockCount;
    bool ret = false;
    if (_entryLock[lockOffset].trywrlock() == 0 ) {
        ret = (node->getRef() == 0);
        if (ret) {
            ret = deleteEntry(entryOffset, node);
        }
        _entryLock[lockOffset].unlock();
    }
    return ret;
}

inline bool LRUHashMap::deleteEntry(
        uint32_t entryOffset, HashListNode* node) 
{
    HashListNode* cur = _entry[entryOffset];
    HashListNode* prev = NULL;
    while(cur && cur->getKey() != node->getKey()) {
        prev = cur;
        cur = cur->nextHashNode();
    }
    
    if (cur) {
        if (NULL == prev) {
            _entry[entryOffset] = cur->nextHashNode();    
        } else {
            prev->setNextHashNode(cur->nextHashNode());
        }
        __sync_sub_and_fetch(&_elemetCount, (uint64_t)1);
    }

    return true;
}

inline HashListNode* LRUHashMap::findAndIncRef(uint64_t key) {
    assert(_entryCount > 0 && _lockCount > 0 
           && NULL != _entryLock && NULL != _entry);

    uint32_t entryOffset = key % _entryCount;
    uint32_t lockOffset = entryOffset % _lockCount;
    ScopedReadLock lock(_entryLock[lockOffset]);
    HashListNode *node = findLockFree(entryOffset, key);
    if (NULL != node) {
        node->addRef();
    }
    return node;
}

inline bool LRUHashMap::findAndInsert(HashListNode* node) {
    assert(_entryCount > 0 && _lockCount > 0 
           && NULL != _entryLock && NULL != _entry);

    if (NULL == node) {
        return false;
    }
    uint32_t entryOffset = node->getKey() % _entryCount;
    uint32_t lockOffset = entryOffset % _lockCount;

    ScopedWriteLock lock(_entryLock[lockOffset]);
    HashListNode *findNode = findLockFree(entryOffset, node->getKey());
    if (NULL == findNode) {
        node->setNextHashNode(_entry[entryOffset]);
        _entry[entryOffset] = node;
        __sync_add_and_fetch(&_elemetCount, (uint64_t)1);
    
        return true;
    } else {
        
        return false;
    }
    
}

inline HashListNode* LRUHashMap::findLockFree(
        uint32_t entryOffset, uint64_t key) const
{
    HashListNode* cur = _entry[entryOffset];
    while(cur && cur->getKey() != key) {
        cur = cur->nextHashNode();
    }
    return cur;
}


inline uint64_t LRUHashMap::size() const {
    return _elemetCount;
}

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_LRUHASHMAP_H
