#ifndef AUTIL_HASHLISTNODE_H
#define AUTIL_HASHLISTNODE_H

#include "autil/CommonMacros.h"
#include <stddef.h>
#include <stdint.h>

AUTIL_BEGIN_NAMESPACE(autil);

class HashListNode
{
public:
    HashListNode();
    ~HashListNode();
private:
    HashListNode(const HashListNode &);
    HashListNode& operator = (const HashListNode &);
public:
    inline void reset();
    
    inline uint32_t addRef();
    inline uint32_t subRef();
    inline uint32_t getRef() const;
    
    inline void setData(void *data, uint32_t len);
    inline uint32_t getData(void *&data) const;
    
    inline void setNextHashNode(HashListNode *nextHashNode);
    inline HashListNode* nextHashNode() const;
    
    inline void setNextListNode(HashListNode *nextListNode);
    inline HashListNode* nextListNode() const;

    inline void setPreListNode(HashListNode *nextListNode);
    inline HashListNode* prevListNode() const;

    inline uint64_t getKey() const;
    inline void setKey(uint64_t key);
private:
    HashListNode *_nextHashNode;
    HashListNode *_nextListNode;
    HashListNode *_prevListNode;

    uint64_t _key;
    void *_data;
    uint32_t _len;
    volatile uint32_t _refCount;
};

inline uint32_t HashListNode::addRef(){
    return __sync_add_and_fetch(&_refCount, (uint32_t)1);
}

inline uint32_t HashListNode::subRef(){
    return __sync_sub_and_fetch(&_refCount, (uint32_t)1);
}

inline  uint32_t HashListNode::getRef() const {
    return _refCount;
}

inline uint64_t HashListNode::getKey() const {
    return _key;
}
inline void HashListNode::setKey(uint64_t key) {
    _key = key;
}

inline void HashListNode::setData(void *data, uint32_t len) {
    _data = data;
    _len = len;
}

inline uint32_t HashListNode::getData(void *&data) const {
    data = _data;
    return _len;
}

inline void HashListNode::setNextHashNode(HashListNode *nextHashNode){
    _nextHashNode = nextHashNode;
}

inline HashListNode* HashListNode::nextHashNode() const {
    return _nextHashNode;
}

inline void HashListNode::setNextListNode(HashListNode *nextListNode) {
    _nextListNode = nextListNode;
}

inline HashListNode* HashListNode::nextListNode() const {
    return _nextListNode;
}

inline void HashListNode::setPreListNode(HashListNode *prevListNode) {
    _prevListNode = prevListNode;
}

inline HashListNode* HashListNode::prevListNode() const {
    return _prevListNode;
}

inline void HashListNode::reset() {
    _nextHashNode = NULL;
    _nextListNode = NULL;
    _prevListNode = NULL;

    _key = 0;
    _data = NULL;
    _len = 0;
    _refCount = 0;
}

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_HASHLISTNODE_H
