#ifndef AUTIL_FIXEDSIZECHUNKALLOCATOR_H
#define AUTIL_FIXEDSIZECHUNKALLOCATOR_H

#include "autil/CommonMacros.h"
#include "autil/ChunkAllocator.h"
#include "autil/FixedSizeAllocator.h"

AUTIL_BEGIN_NAMESPACE(autil);

class FixedSizeChunkAllocator : public ChunkAllocator
{
public:
    FixedSizeChunkAllocator();
    ~FixedSizeChunkAllocator();
private:
    FixedSizeChunkAllocator(const FixedSizeChunkAllocator &);
    FixedSizeChunkAllocator& operator = (const FixedSizeChunkAllocator &);
public:
    void init(uint32_t requestSize, uint32_t maxRequestChunk);
public:
    /* override */ void* allocate(uint32_t size);
    /* override */ void free(void* const addr);
    /* override */ void release();
private:
    size_t _fixedSize;
    char *_free;
    char *_end;
    void *_freeList;
    size_t _count;
    size_t _maxRequestChunk;
    char *_buffer;
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_FIXEDSIZECHUNKALLOCATOR_H
