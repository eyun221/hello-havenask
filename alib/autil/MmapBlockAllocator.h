#ifndef AUTIL_MMAPBLOCKALLOCATOR_H
#define AUTIL_MMAPBLOCKALLOCATOR_H

#include "autil/CommonMacros.h"
#include "autil/Lock.h"
#include "autil/Allocators.h"
#include "autil/BlockAllocator.h"
#include "autil/FixedSizeAllocator.h"
#include "autil/FixedSizeRecyclePool.h"

AUTIL_BEGIN_NAMESPACE(autil);

class MmapBlockAllocator : public BlockAllocator
{
public:
    MmapBlockAllocator(uint32_t blockSize, size_t blockCount);
    ~MmapBlockAllocator();
private:
    MmapBlockAllocator(const MmapBlockAllocator &);
    MmapBlockAllocator& operator = (const MmapBlockAllocator &);
public:
    /* override */ Block* allocBlock();
    /* override */ void freeBlock(Block* block);
private:
    ThreadMutex _lock;
    FixedSizeAllocator _blockHeaderAllocator;
    FixedSizeRecyclePool<MmapAllocator> _blockDataPool;
};

typedef std::tr1::shared_ptr<MmapBlockAllocator> MmapBlockAllocatorPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_MMAPBLOCKALLOCATOR_H
