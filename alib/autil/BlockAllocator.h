#ifndef AUTIL_BLOCKALLOCATOR_H
#define AUTIL_BLOCKALLOCATOR_H

#include <tr1/memory>
#include "autil/CommonMacros.h"
#include "autil/Block.h"

AUTIL_BEGIN_NAMESPACE(autil);

class BlockAllocator
{
public:
    BlockAllocator(uint32_t blockSize) {}
    virtual ~BlockAllocator() {}
private:
    BlockAllocator(const BlockAllocator &);
    BlockAllocator& operator = (const BlockAllocator &);
public:
    virtual Block* allocBlock() = 0;
    virtual void freeBlock(Block* block) = 0;
};

typedef std::tr1::shared_ptr<BlockAllocator> BlockAllocatorPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_BLOCKALLOCATOR_H
