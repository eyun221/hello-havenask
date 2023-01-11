#ifndef AUTIL_REPLACEPOLICY_H
#define AUTIL_REPLACEPOLICY_H

#include "autil/CommonMacros.h"
#include "autil/BlockLinkListNode.h"
#include "autil/DoubleLinkList.h"
#include "autil/ReferencedHashMap.h"
#include "autil/BlockAllocator.h"

AUTIL_BEGIN_NAMESPACE(autil);

class ReplacePolicy
{
public:
    struct TryReplacePredicate
    {
    public:
        TryReplacePredicate(ReferencedHashMap<blockid_t, BlockLinkListNode*>& blockMap)
            :_blockMap(blockMap)
        {
        }
        bool operator()(BlockLinkListNode* node)
        {
            bool ret =  _blockMap.tryDelete((BlockLinkListNode*)node);
            return ret;
        }

    private:
        ReferencedHashMap<blockid_t, BlockLinkListNode*>& _blockMap;
    };

public:
    ReplacePolicy() {}
    virtual ~ReplacePolicy() {}

private:
    ReplacePolicy(const ReplacePolicy &);
    ReplacePolicy& operator = (const ReplacePolicy &);
public:
    virtual void accessBlock(BlockLinkListNode* blockNode) = 0;
    virtual uint32_t replaceBlocks(TryReplacePredicate& tryReplace,
                                   BlockLinkListNode*& firstReplacedBlock) = 0;
};

typedef std::tr1::shared_ptr<ReplacePolicy> ReplacePolicyPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_REPLACEPOLICY_H
