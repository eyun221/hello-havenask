#ifndef AUTIL_SIMPLE_ALLOCATOR_H
#define AUTIL_SIMPLE_ALLOCATOR_H

#include <assert.h>
#include "autil/CommonMacros.h"
#include "autil/mem_pool/ChunkAllocatorBase.h"

namespace autil { namespace mem_pool {

class SimpleAllocator : public ChunkAllocatorBase
{
public:
    SimpleAllocator();
    ~SimpleAllocator();

public:
    virtual void* doAllocate(size_t numBytes)
    {
        return static_cast<void*>(new (std::nothrow) char[numBytes]);
    }
    virtual void doDeallocate(void* const addr, size_t numBytes)
    {
        delete[] (char*)addr;
    }
};

typedef std::tr1::shared_ptr<SimpleAllocator> SimpleAllocatorPtr;

}}

#endif //AUTIL_SIMPLE_ALLOCATOR_H
