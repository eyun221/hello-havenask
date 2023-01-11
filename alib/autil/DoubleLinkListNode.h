#ifndef AUTIL_DOUBLE_LINK_LIST_NODE_H
#define AUTIL_DOUBLE_LINK_LIST_NODE_H

#include "autil/CommonMacros.h"

AUTIL_BEGIN_NAMESPACE(autil);

template <class T>
class DoubleLinkListNode
{
public:
    DoubleLinkListNode();

public:
    void reset();

private:
    DoubleLinkListNode(const DoubleLinkListNode &);
public:
    DoubleLinkListNode<T> *_nextListNode;
    DoubleLinkListNode<T> *_prevListNode;
    T _data;
};

////////////////////////////////////////////////////
template<class T>
DoubleLinkListNode<T>::DoubleLinkListNode()
{
    reset();
}

template<class T>
inline void DoubleLinkListNode<T>::reset() {
    _nextListNode = NULL;
    _prevListNode = NULL;
}

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_DOUBLE_LINK_LIST_NODE_H
