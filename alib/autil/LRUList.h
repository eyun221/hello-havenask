#ifndef AUTIL_LRULIST_H
#define AUTIL_LRULIST_H

#include "autil/CommonMacros.h"
#include "autil/HashListNode.h"
#include "autil/Lock.h"

AUTIL_BEGIN_NAMESPACE(autil);

class LRUList
{
public:
    LRUList();
    ~LRUList();
private:
    LRUList(const LRUList &);
    LRUList& operator = (const LRUList &);
public:
    inline void pushFront(HashListNode *listNode);

    inline void moveToHead(HashListNode *listNode);
    
    inline void deleteNode(HashListNode *listNode);

    inline HashListNode *getNoneRefrencedNode();// reference count is zero;

    inline uint32_t getElementCount() const;
private:
    inline void deleteNodeLockFree(HashListNode *listNode);
    inline void pushFrontLockFree(HashListNode *listNode);
private:
    HashListNode *_head;
    HashListNode *_tail;

    ThreadMutex _lock;
    uint32_t _elementCount;
private:
    friend class LRUListTest;
};

inline void LRUList::pushFront(HashListNode *listNode) {
    assert(NULL != listNode);
    ScopedLock lock(_lock);
    pushFrontLockFree(listNode);
    ++_elementCount;
}

inline void LRUList::moveToHead(HashListNode *listNode) {
    assert(NULL != listNode);
    ScopedLock lock(_lock);
    deleteNodeLockFree(listNode);
    pushFrontLockFree(listNode);
}

inline void LRUList::deleteNode(HashListNode *listNode) {
    assert(NULL != listNode);
    ScopedLock lock(_lock);
    deleteNodeLockFree(listNode);
    --_elementCount;
}

inline HashListNode *LRUList::getNoneRefrencedNode() {
    ScopedLock lock(_lock);
    HashListNode *node = _tail;
    int32_t i = 1;
    while (i < 32 && NULL != node && 0 != node->getRef()) {
        node = node->prevListNode();
        ++i;
    }
    return ( (NULL != node) && (0 == node->getRef()) ) ? node : NULL;
}

inline uint32_t  LRUList::getElementCount() const {
    return _elementCount;
}

inline void LRUList::deleteNodeLockFree(HashListNode *listNode) {
    assert(NULL != listNode);
    
    if (listNode->nextListNode()) {
        listNode->nextListNode()->setPreListNode(listNode->prevListNode());
    }
    if (listNode->prevListNode()) {
        listNode->prevListNode()->setNextListNode(listNode->nextListNode());
    }
    if (_head == listNode) {
        _head = listNode->nextListNode();
    }
    if (_tail == listNode) {
        _tail = listNode->prevListNode();
    }
}

inline void LRUList::pushFrontLockFree(HashListNode *listNode) {
    assert(NULL != listNode);

    listNode->setNextListNode(NULL);
    listNode->setPreListNode(NULL);


    if (NULL == _head && NULL == _tail) {
        _head = listNode;
        _tail = listNode;
    } else {
        listNode->setNextListNode(_head);
        _head->setPreListNode(listNode);
        _head = listNode;
    }
}

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_LRULIST_H
