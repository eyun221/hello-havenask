#ifndef AUTIL_CHUNK_ALLOCATOR_H
#define AUTIL_CHUNK_ALLOCATOR_H

#include <tr1/memory>
#include "autil/CommonMacros.h"

namespace autil { namespace mem_pool {

class ChunkAllocatorBase
{
public:
    ChunkAllocatorBase(): _usedBytes(0) {}
    virtual ~ChunkAllocatorBase() {}

public:
    void* allocate(size_t numBytes) 
    {
        void *p = doAllocate(numBytes);
        _usedBytes += numBytes;
        return p;
    }
    void deallocate(void* const addr, size_t numBytes)
    {
        doDeallocate(addr, numBytes);
        _usedBytes -= numBytes;
    }
    size_t getUsedBytes() const {return _usedBytes;}

private:
    virtual void* doAllocate(size_t numBytes) = 0;
    virtual void doDeallocate(void* const addr, size_t numBytes) = 0;

protected:
    size_t _usedBytes;
};

}}

#endif //AUTIL_CHUNK_ALLOCATOR_H
