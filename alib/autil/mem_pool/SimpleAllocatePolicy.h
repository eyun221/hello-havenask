#ifndef AUTIL_SIMPLE_ALLOCATE_POLICY_TYPED_H
#define AUTIL_SIMPLE_ALLOCATE_POLICY_TYPED_H

#include "autil/CommonMacros.h"
#include "autil/Log.h"
#include "autil/mem_pool/MemoryChunk.h"
#include "autil/mem_pool/AllocatePolicy.h"
#include "autil/mem_pool/ChunkAllocatorBase.h"

namespace autil { namespace mem_pool {

class SimpleAllocatePolicy : public AllocatePolicy
{
public:
    SimpleAllocatePolicy(ChunkAllocatorBase* allocator, size_t chunkSize);
    SimpleAllocatePolicy(size_t chunkSize);
    ~SimpleAllocatePolicy();

public:
    /** 
     * alloc a chunk
     * @param nBytes number of bytes to allocate
     * @return allocated chunk
     */
    MemoryChunk* allocate(size_t nBytes);

    /**
     * release allocated chunks
     * @return total size of chunks
     */
    size_t release();

    /**
     * Reset memory to init state
     */
    size_t reset();

    /**
     * Get used chunk size
     */
    size_t getUsedBytes() const;

    /**
     * Get total chunk size
     */
    size_t getTotalBytes() const;

    /**
     * Return chunk size
     */
    size_t getChunkSize() const;

    /**
     * Return available chunk size, it is always 
     * equal to GetChunkSize() - sizeof(ChainedMemoryChunk)
     */
    size_t getAvailableChunkSize() const;

    /*
     * check whether pointer in pool.
     */
    bool isInChunk(const void *ptr) const;

private:
    ChunkAllocatorBase* _allocator;
    bool _ownAllocator;

    ChainedMemoryChunk* _chunkHeader;
    ChainedMemoryChunk* _currentChunk;
    size_t _chunkSize;
    size_t _usedBytes;
    size_t _totalBytes;
private:
    AUTIL_LOG_DECLARE();
};

/////////////////////////////////////////////////////////////////////////
//

inline MemoryChunk* SimpleAllocatePolicy::allocate(size_t nBytes)
{
    size_t nAllocSize = nBytes + sizeof(ChainedMemoryChunk);
    if (nAllocSize <= _chunkSize)
    {
        nAllocSize = _chunkSize;
    }

    ChainedMemoryChunk* nextChunk = _currentChunk ? _currentChunk->next() : _chunkHeader;
    if (nextChunk && nAllocSize <= _chunkSize)
    {
        _currentChunk = nextChunk;
    }
    else 
    {
        nextChunk = new (_allocator->allocate(nAllocSize)) ChainedMemoryChunk(nAllocSize);

        if (!nextChunk)
        {
            return NULL;
        }
        _totalBytes += nAllocSize;
        if (!_chunkHeader)
        {
            _currentChunk = _chunkHeader = nextChunk;
        }
        else 
        {
            if (_currentChunk) { 
                nextChunk->next() = _currentChunk->next();
                if (_currentChunk->next()) {
                    _currentChunk->next()->prev() = nextChunk;
                }
                _currentChunk->next() = nextChunk;
                nextChunk->prev() = _currentChunk;
            } else { 
                nextChunk->next() = _chunkHeader;
                _chunkHeader->prev() = nextChunk;
                _chunkHeader = nextChunk;
            }
            _currentChunk = nextChunk;
        }
    }
    _usedBytes += _currentChunk->getTotalBytes();
    return _currentChunk;
}

inline size_t SimpleAllocatePolicy::release()
{
    ChainedMemoryChunk* pChunk = _chunkHeader;
    ChainedMemoryChunk* pChunk2 = NULL;
    while (pChunk)
    {
        pChunk2 = pChunk;
        pChunk = pChunk2->next();
        _allocator->deallocate((void*)pChunk2, pChunk2->getTotalBytes());
    }
    _chunkHeader = _currentChunk = NULL;
    size_t totalBytes = _totalBytes;
    _usedBytes = _totalBytes = 0;
    return totalBytes;
}

inline size_t SimpleAllocatePolicy::reset()
{
    size_t totalBytes = _totalBytes;
    for (ChainedMemoryChunk* chunk = _chunkHeader; chunk; )
    {
        if (chunk->getTotalBytes() <= _chunkSize)
        {
            chunk->reset();
            chunk = chunk->next();
        }
        else
        {
            ChainedMemoryChunk* prevChunk = chunk->prev();
            ChainedMemoryChunk* nextChunk = chunk->next();
            _totalBytes -= chunk->getTotalBytes();
            _allocator->deallocate((void*)chunk, chunk->getTotalBytes());
            chunk = nextChunk;
            if (prevChunk)
            {
                prevChunk->next() = nextChunk;
            }
            else
            {
                _chunkHeader = nextChunk;
            }
            if (nextChunk)
            {
                nextChunk->prev() = prevChunk;
            }
        }
    }
    _usedBytes = 0;
    _currentChunk = NULL;
    return totalBytes;
}

inline size_t SimpleAllocatePolicy::getUsedBytes() const 
{
    return _usedBytes;
}

inline size_t SimpleAllocatePolicy::getTotalBytes() const 
{
    return _totalBytes;
}

inline size_t SimpleAllocatePolicy::getChunkSize() const 
{
    return _chunkSize;
}

inline size_t SimpleAllocatePolicy::getAvailableChunkSize() const 
{
    return _chunkSize - sizeof(ChainedMemoryChunk);
}

inline bool SimpleAllocatePolicy::isInChunk(const void *ptr) const {
    ChainedMemoryChunk* chunk = _chunkHeader;
    while (chunk) {
        if (chunk->isInChunk(ptr)) {
            return true;
        }
        chunk = chunk->next();
    }
    return false;
}

}}

#endif //AUTIL_SIMPLE_ALLOCATE_POLICY_H
