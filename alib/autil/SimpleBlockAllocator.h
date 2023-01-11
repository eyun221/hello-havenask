#ifndef AUTIL_SIMPLEBLOCKALLOCATOR_H
#define AUTIL_SIMPLEBLOCKALLOCATOR_H

#include <tr1/memory>
#include "autil/BlockAllocator.h"
#include "autil/FixedSizeAllocator.h"
#include "autil/Lock.h"
#include "autil/CommonMacros.h"

AUTIL_BEGIN_NAMESPACE(autil);

class SimpleBlockAllocator : public BlockAllocator
{
public:
    SimpleBlockAllocator(uint32_t blockSize);
    virtual ~SimpleBlockAllocator();

private:
    SimpleBlockAllocator(const SimpleBlockAllocator &);
    SimpleBlockAllocator& operator = (const SimpleBlockAllocator &);

public:
    /* override */ Block* allocBlock();
    /* override */ void freeBlock(Block* block);

public:
    // for testing
    size_t getAllocatedCount() const {return _blockHeaderAllocator.getCount();}

private:
    void clear();

private:
    FixedSizeAllocator _blockHeaderAllocator;
    FixedSizeAllocator _blockDataAllocator;
    ThreadMutex _blockAllocatorLock;
};

typedef std::tr1::shared_ptr<SimpleBlockAllocator> SimpleBlockAllocatorPtr;

///////////inline methods
///////////////////////////////////////////////

inline void SimpleBlockAllocator::clear()
{
    _blockHeaderAllocator.clear();
    _blockDataAllocator.clear();
}

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_SIMPLEBLOCKALLOCATOR_H
