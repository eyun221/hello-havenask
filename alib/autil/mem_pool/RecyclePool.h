#ifndef AUTIL_RECYCLE_POOL_H
#define AUTIL_RECYCLE_POOL_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#if __cplusplus >= 201103L
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif
#include <tr1/memory>
#include "autil/SimpleSegregatedAllocator.h"
#include "autil/mem_pool/MemoryChunk.h"
#include "autil/mem_pool/SimpleAllocatePolicy.h"
#include "autil/mem_pool/Pool.h"


namespace autil { namespace mem_pool {

class RecyclePool : public Pool
{
public:
#if __cplusplus >= 201103L
    typedef std::unordered_map<size_t, void *> HashMap;
#else
    typedef std::tr1::unordered_map<size_t, void *> HashMap;
#endif
public:
    RecyclePool(ChunkAllocatorBase* allocator, size_t chunkSize,
                size_t alignSize = 1);
    RecyclePool(size_t chunkSize, size_t alignSize = 1); 
    ~RecyclePool();

private:
    RecyclePool(const RecyclePool&);
    void operator=(const RecyclePool&);

public:
    /**
     * numBytes >= 8
     */
    virtual void* allocate(size_t numBytes);

    /*
     * only less than 64k memory block can be deallocated 
     */
    virtual void deallocate(void *ptr, size_t size);    
    virtual void release();
    virtual size_t reset();

    size_t getFreeSize() const;

public:
    //for test
    HashMap& getFreeList();

private:
    static void*& nextOf(void *ptr);

private:
    size_t _freeSize;
    HashMap _freeList;

private:
    AUTIL_LOG_DECLARE();
};

//////////////////////////////////////////////////
//implementation

inline void*& RecyclePool::nextOf(void *ptr)
{
    return *(static_cast<void **>(ptr));
}

inline void* RecyclePool::allocate(size_t numBytes)
{
    size_t allocSize = alignBytes(numBytes, _alignSize);
    if (allocSize < 8) {
        allocSize = 8;
    }
    HashMap::iterator it = _freeList.find(allocSize);
    if (it != _freeList.end()) {
        void* ptr = it->second;
        if (ptr != NULL) {
            _freeList[allocSize] = nextOf(ptr);
            _freeSize -= allocSize;
            _allocSize += allocSize;
            return ptr;
        }
    }
    return Pool::allocate(allocSize);
}

inline void RecyclePool::deallocate(void *ptr, size_t size)
{
    size_t allocSize = alignBytes(size, _alignSize);
    if (allocSize < 8) {
        allocSize = 8;
    }

    HashMap::iterator it = _freeList.find(allocSize);
    if (it == _freeList.end()) {
        _freeList[allocSize] = NULL;
    }
    nextOf(ptr) = _freeList[allocSize];
    _freeList[allocSize] = ptr;
    _allocSize -= allocSize;
    _freeSize += allocSize;
}

inline void RecyclePool::release()
{
    _freeSize = 0;
    _freeList.clear();

    Pool::release();
}

inline size_t RecyclePool::reset()
{
    _freeSize = 0;
    _freeList.clear();

    return Pool::reset();
}

inline size_t RecyclePool::getFreeSize() const
{
    return _freeSize;
}

inline RecyclePool::HashMap& RecyclePool::getFreeList()
{
    return _freeList;
}

}}

#endif //AUTIL_RECYCLE_POOL_H
