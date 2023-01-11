#ifndef AUTIL_HASHNODEPOOL_H
#define AUTIL_HASHNODEPOOL_H

#include "autil/CommonMacros.h"
#include "autil/HashListNode.h"
#include "autil/Lock.h"

AUTIL_BEGIN_NAMESPACE(autil);

class HashNodePool
{
public:
    HashNodePool();
    ~HashNodePool();
private:
    HashNodePool(const HashNodePool &);
    HashNodePool& operator = (const HashNodePool &);
public:
    bool init(uint32_t maxCount);

    inline HashListNode *allocNode();
    inline void freeNode(HashListNode *node);

    inline uint64_t getFreeCount() const;
private:
    HashListNode *_allocatedNode;
    HashListNode *_freeHeader;
    volatile uint64_t _freeCount;
    ThreadMutex _lock;
    
    friend class HashNodePoolTest;
};

inline HashListNode *HashNodePool::allocNode() {
    ScopedLock lock(_lock);
    if (_freeCount > 0) {
        HashListNode *node = _freeHeader;
        _freeHeader = node->nextHashNode();
        --_freeCount;
        node->setNextHashNode(NULL);
        return node;
    }
    return NULL;
}

inline void HashNodePool::freeNode(HashListNode *node) {
    if (NULL == node) {
        return;
    }
    ScopedLock lock(_lock);
    node->setNextHashNode(_freeHeader);
    _freeHeader = node;
    ++_freeCount;
}

inline uint64_t HashNodePool::getFreeCount() const {
    return _freeCount;
}

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_HASHNODEPOOL_H
