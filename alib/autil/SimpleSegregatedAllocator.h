#ifndef AUTIL_SIMPLESEGREGATEDALLOCATOR_H
#define AUTIL_SIMPLESEGREGATEDALLOCATOR_H

#include "autil/CommonMacros.h"
#include "autil/ChunkAllocator.h"
#include <tr1/memory>

AUTIL_BEGIN_NAMESPACE(autil);

class SimpleSegregatedAllocator : public ChunkAllocator
{
private:
    SimpleSegregatedAllocator(const SimpleSegregatedAllocator&);
    void operator=(const SimpleSegregatedAllocator&);
public:
    /** constructor */
    SimpleSegregatedAllocator();
    /** destructor */
    virtual ~SimpleSegregatedAllocator();

public:
    /**
     * Initialize and allocate memory
     */
    bool init(uint32_t requestSize, uint32_t maxRequestChunk);

    /**
     * allocate one memory chunk from this pool
     * @return allocated memory address
     */
    void* allocate(uint32_t size);

    /**
     * free memory chunk which was previously returned from alloc()
     * @param pAddr chunk to free
     */
    void free(void* const pAddr);

    /**
     * release all allocated memory
     */
    void release();

    uint32_t getRequestSize() const;

protected:
    /**
     * grow the list
     * @param min chunks
     * @return false if allocate memory failed.
     */
    virtual bool allocateChunk();

protected:
    /** 
     * get next node of pAddr
     * @param pAddr pointer
     * @return next node 
     */
    inline static void*& nextOf(void* const pAddr)
    {return *(static_cast<void**>(pAddr));}

    /**
     * segregate a chunk to partitions
     * @param pChunk memory chunk
     * @param nChunkSize size of chunk
     * @param requestedSize size of partition
     * @param pEndNode the last node to link to
     * @return first node of partitions
     */
    static void* segregate(void* pChunk, uint64_t nChunkSize, 
                           uint32_t requestedSize, void* pEndNode = NULL);

protected:
    uint64_t _chunkSize;
private:
    void* _first; ///first free node
    uint32_t _requestSize;///request size
    void * _memChunk;
};

typedef std::tr1::shared_ptr<SimpleSegregatedAllocator> SimpleSegregatedAllocatorPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_SIMPLESEGREGATEDALLOCATOR_H
