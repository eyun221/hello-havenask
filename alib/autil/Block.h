#ifndef __AUTIL_BLOCK_H
#define __AUTIL_BLOCK_H

#include <stddef.h>
#include <stdint.h>
#include "autil/CommonMacros.h"

AUTIL_BEGIN_NAMESPACE(autil);

typedef uint64_t blockid_t;
#define INVALID_BLOCKID (blockid_t)(-1)

class BlockCache;
class Block
{
public:
    Block();
    ~Block();
public:
    uint32_t incRefCount();
    uint32_t decRefCount();
    uint32_t getRefCount() const;

public:
    blockid_t _id;
    uint8_t* _data;

private:
    uint32_t _refCount;
};

/////////////////////////////////////////

inline Block::Block()
    : _id(INVALID_BLOCKID)
    , _data(NULL)
    , _refCount(0)
{
}

inline Block::~Block()
{
}

inline uint32_t Block::incRefCount()
{
    return __sync_add_and_fetch(&_refCount, (uint32_t)1);
}

inline uint32_t Block::getRefCount() const
{
    return _refCount;
}

inline uint32_t Block::decRefCount()
{
    return __sync_sub_and_fetch(&_refCount, (uint32_t)1);
}


AUTIL_END_NAMESPACE(autil);

#endif //__AUTIL_BLOCK_H
