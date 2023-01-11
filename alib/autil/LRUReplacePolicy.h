#ifndef AUTIL_LRUREPLACEPOLICY_H
#define AUTIL_LRUREPLACEPOLICY_H

#include "autil/CommonMacros.h"
#include "autil/Lock.h"
#include "autil/ReplacePolicy.h"

AUTIL_BEGIN_NAMESPACE(autil);

class LRUReplacePolicy : public ReplacePolicy
{
public:
    LRUReplacePolicy(uint32_t maxBlockCountPerReplacement, uint32_t maxScanCountPerReplacement);
    ~LRUReplacePolicy();

private:
    LRUReplacePolicy(const LRUReplacePolicy &);
    LRUReplacePolicy& operator = (const LRUReplacePolicy &);

public:
    /* override */ void accessBlock(BlockLinkListNode* blockNode);
    /* override */ uint32_t replaceBlocks(TryReplacePredicate& tryReplace,
            BlockLinkListNode*& firstReplacedBlock);

    // for test
    size_t getLRUListLength() const {return _accessQueue.getElementCount();}
    BlockLinkListNode* getHead() const {return (BlockLinkListNode*) _accessQueue.getHead();}

private:
    void clear();

private:
    uint32_t _maxBlockCountPerReplacement;
    uint32_t _maxScanCountPerReplacement;

    ThreadMutex _lock;
    DoubleLinkList<Block*> _accessQueue;
    friend class LRUReplacePolicyTest;
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_LRUREPLACEPOLICY_H
