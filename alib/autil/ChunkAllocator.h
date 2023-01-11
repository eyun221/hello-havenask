#ifndef AUTIL_CHUNKALLOCATOR_H
#define AUTIL_CHUNKALLOCATOR_H

#include "autil/CommonMacros.h"
#include <stdint.h>

AUTIL_BEGIN_NAMESPACE(autil);

class ChunkAllocator 
{
public:
    virtual ~ChunkAllocator() {}
public:
    virtual void* allocate(uint32_t size) = 0;
    virtual void free(void* const addr) = 0;
    virtual void release() = 0;
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_CHUNKALLOCATOR_H
