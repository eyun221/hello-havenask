#ifndef AUTIL_BLOCKLINKLISTNODE_H
#define AUTIL_BLOCKLINKLISTNODE_H

#include "autil/CommonMacros.h"
#include "autil/Block.h"
#include "autil/DoubleLinkListNode.h"

AUTIL_BEGIN_NAMESPACE(autil);

class BlockLinkListNode : public DoubleLinkListNode<Block*>
{
public:
    void setKey(blockid_t id) {_data->_id = id;}
    blockid_t getKey() const { return _data->_id; }
    uint32_t getRefCount() const { return _data->getRefCount();}
    uint32_t incRefCount() { return _data->incRefCount(); }
    uint32_t decRefCount() { return _data->decRefCount();}
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_BLOCKLINKLISTNODE_H
