#ifndef AUTIL_POOL_H
#define AUTIL_POOL_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "autil/Log.h"
#include "autil/Lock.h"
#include "autil/mem_pool/MemoryChunk.h"
#include "autil/mem_pool/SimpleAllocatePolicy.h"
#include "autil/mem_pool/PoolBase.h"
#include <iostream>

namespace autil { namespace mem_pool {

class Pool : public PoolBase
{
public:
    static const size_t DEFAULT_CHUNK_SIZE = 10 * 1024 * 1024; // 10M
    static const size_t DEFAULT_ALIGN_SIZE = sizeof(char*);
public:
    Pool(ChunkAllocatorBase* allocator, size_t chunkSize,
         size_t alignSize = DEFAULT_ALIGN_SIZE);

    Pool(size_t chunkSize = DEFAULT_CHUNK_SIZE,
         size_t alignSize = DEFAULT_ALIGN_SIZE);

    virtual ~Pool();

private:
    Pool(const Pool&);
    void operator = (const Pool&);

public:
    /**
     * Allocate memory from Pool
     * @param numBytes number of bytes need to allocate
     * @return address of allocated memory
     */
    virtual void* allocate(size_t numBytes);

    void* allocateUnsafe(size_t numBytes);

    /**
     * Free memory, dummy function
     */
    virtual void deallocate(void*, size_t) {}

    /**
     * release all allocated memory in Pool
     */
    virtual void release();

    /**
     * Reset memory to init state
     */
    virtual size_t reset();
    size_t resetUnsafe();

    void *allocateAlign(size_t numBytes, size_t alignSize);
    /**
     * get alignment size
     * @return alignSize alignment size
     */
    size_t getAlignSize() const {return _alignSize;}

    /**
     * Get used size in bytes
     */
    size_t getUsedBytes() const;

    /**
     * Get total size in bytes
     */
    size_t getTotalBytes() const;

    /**
     *Get allocated size from pool
     */
    size_t getAllocatedSize() const;

    size_t getAvailableChunkSize() const;

    /*
     * Check whether pointer in pool
     */
    bool isInPool(const void *ptr) const;

    static size_t alignBytes(size_t numBytes, size_t alignSize);

protected:
    mutable SpinLock _mutex;
    size_t _alignSize; ///Alignment length
    MemoryChunk* _memChunk;  ///Memory chunk
    size_t _allocSize;// size of allocated memory from this pool

    AllocatePolicy* _allocPolicy;///allocate policy
    static MemoryChunk DUMMY_CHUNK;///dummy chunk
private:
    AUTIL_LOG_DECLARE();
};

class UnsafePool : public Pool {
public:
    UnsafePool(ChunkAllocatorBase* allocator, size_t chunkSize,
         size_t alignSize = DEFAULT_ALIGN_SIZE);

    UnsafePool(size_t chunkSize = DEFAULT_CHUNK_SIZE,
               size_t alignSize = DEFAULT_ALIGN_SIZE);
private:
    UnsafePool(const UnsafePool&);
    void operator = (const UnsafePool&);
public:
    void* allocate(size_t numBytes) override {
        return allocateUnsafe(numBytes);
    }
    size_t reset() override {
        return resetUnsafe();
    }
};

//////////////////////////////////////////////////
//implementation

inline void *Pool::allocateAlign(size_t numBytes, size_t alignSize) {
    numBytes += alignSize;
    static_assert(sizeof(size_t) == sizeof(void*), "size not equal");
    return (void*)alignBytes((size_t)allocate(numBytes), alignSize);
}

inline void Pool::release()
{
    ScopedSpinLock lock(_mutex);
    _allocPolicy->release();
    _allocSize = 0;
    _memChunk = &(Pool::DUMMY_CHUNK);
}

inline size_t Pool::reset()
{
    ScopedSpinLock lock(_mutex);
    return resetUnsafe();
}

inline size_t Pool::resetUnsafe() {
    size_t totalSize = _allocPolicy->reset();
    _memChunk = &(Pool::DUMMY_CHUNK);
    _allocSize = 0;
    return totalSize;
}

inline size_t Pool::getUsedBytes() const
{
    ScopedSpinLock lock(_mutex);
    size_t allocBytes = _allocPolicy->getUsedBytes();
    allocBytes -= _memChunk->getFreeSize();
    return allocBytes;
}

inline size_t Pool::getAllocatedSize() const
{
    ScopedSpinLock lock(_mutex);
    return _allocSize;
}

inline size_t Pool::getTotalBytes() const
{
    ScopedSpinLock lock(_mutex);
    return _allocPolicy->getTotalBytes();
}

inline size_t Pool::getAvailableChunkSize() const
{
    ScopedSpinLock lock(_mutex);
    return _allocPolicy->getAvailableChunkSize();
}

inline size_t Pool::alignBytes(size_t numBytes, size_t alignSize)
{
    return ((numBytes + alignSize - 1) & ~(alignSize - 1));
}

}}

#endif
