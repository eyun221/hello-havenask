#ifndef AUTIL_ALLOCATE_POLICY_H
#define AUTIL_ALLOCATE_POLICY_H

#include <tr1/memory>
#include "autil/CommonMacros.h"

namespace autil { namespace mem_pool {

class AllocatePolicy
{
public:
    AllocatePolicy() {}
    virtual ~AllocatePolicy() {}
public:
        /** 
     * alloc a chunk
     * @param nBytes number of bytes to allocate
     * @return allocated chunk
     */
    virtual MemoryChunk* allocate(size_t nBytes) = 0;

    /**
     * release allocated chunks
     * @return total size of chunks
     */
    virtual size_t release() = 0;

    /**
     * Reset memory to init state
     */
    virtual size_t reset() = 0;

    /**
     * Get used chunk size
     */
    virtual size_t getUsedBytes() const = 0;

    /**
     * Get total chunk size
     */
    virtual size_t getTotalBytes() const = 0;

    /**
     * Return chunk size
     */
    virtual size_t getChunkSize() const = 0;

    /**
     * Return available chunk size, it is always 
     * equal to GetChunkSize() - sizeof(ChainedMemoryChunk)
     */
    virtual size_t getAvailableChunkSize() const = 0;
    
    /*
     * check whether pointer in chunk.
     */
    virtual bool isInChunk(const void *ptr) const = 0;
};

}}

#endif //AUTIL_ALLOCATE_POLICY_H
