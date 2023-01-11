#ifndef AUTIL_GENERALRECLAIMER_H
#define AUTIL_GENERALRECLAIMER_H

#include "autil/CommonMacros.h"
#include "autil/BlockPool.h"

AUTIL_BEGIN_NAMESPACE(autil);

class GeneralReclaimer
{
public:
    GeneralReclaimer();
    ~GeneralReclaimer();
private:
    GeneralReclaimer& operator = (const GeneralReclaimer &);
public:
    void operator() (void *data, uint64_t len, BlockPool &pool, uint64_t &actualLen) const {
        if (NULL == data) {
            return;
        }

        uint32_t blockSize = pool.getBlockSize();
        std::vector<void*> toReclaimBlocks;
        uint32_t actualBlockDataLen = blockSize - sizeof(char*);
        assert(actualBlockDataLen > 0);
        size_t reclaimBlockNum = (len + actualBlockDataLen -1) / actualBlockDataLen;
        toReclaimBlocks.reserve(reclaimBlockNum);
        
        char *curBlock = (char *)data;
        uint64_t reclaimedLen = 0;
        actualLen = 0;
        while(len > reclaimedLen) {
            char *nextBlock =(char *)(((MemBlock *)(curBlock + actualBlockDataLen))->_next);
            toReclaimBlocks.push_back(curBlock);
            curBlock = nextBlock;
            reclaimedLen += actualBlockDataLen;
            actualLen += blockSize;
        }

        pool.freeBlock(toReclaimBlocks);
    }
private:

};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_GENERALRECLAIMER_H
