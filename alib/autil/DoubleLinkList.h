#ifndef AUTIL_DOUBLE_LINK_LIST_H
#define AUTIL_DOUBLE_LINK_LIST_H

#include "autil/CommonMacros.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "autil/DoubleLinkListNode.h"

AUTIL_BEGIN_NAMESPACE(autil);

template<class T>
class DoubleLinkList
{
public:
    DoubleLinkList();
    ~DoubleLinkList();
private:
    DoubleLinkList(const DoubleLinkList<T> &);
    DoubleLinkList& operator = (const DoubleLinkList<T> &);

public:
    inline void pushFront(DoubleLinkListNode<T> *listNode);
    inline void moveToHead(DoubleLinkListNode<T> *listNode);
    inline void deleteNode(DoubleLinkListNode<T> *listNode);
    inline uint32_t getElementCount() const;
    inline DoubleLinkListNode<T> *getTail() const;
    inline DoubleLinkListNode<T> *getHead() const;
private:
    DoubleLinkListNode<T> *_head;
    DoubleLinkListNode<T> *_tail;

    uint32_t _elementCount;
};

//////////////////////////////////////////////////////////
template<class T>
inline DoubleLinkList<T>::DoubleLinkList()
    : _head(NULL)
    , _tail(NULL)
    , _elementCount(0)
{
}

template<class T>
inline DoubleLinkList<T>::~DoubleLinkList()
{
}

template<class T>
inline void DoubleLinkList<T>::pushFront(DoubleLinkListNode<T> *listNode) 
{
    assert(NULL != listNode);
 
    listNode->_nextListNode = NULL;
    listNode->_prevListNode = NULL;

    if (NULL == _head && NULL == _tail) 
    {
        _head = listNode;
        _tail = listNode;
    }
    else 
    {
        listNode->_nextListNode = _head;
        _head->_prevListNode = listNode;
        _head = listNode;
    }
    ++_elementCount;
}

template<class T>
inline void DoubleLinkList<T>::moveToHead(DoubleLinkListNode<T> *listNode) 
{
    assert(NULL != listNode);
    deleteNode(listNode);
    pushFront(listNode);
}

template<class T>
inline void DoubleLinkList<T>::deleteNode(DoubleLinkListNode<T> *listNode) 
{
    assert(NULL != listNode);
    
    if (listNode->_nextListNode == NULL
        && listNode->_prevListNode == NULL
        && _head != listNode) // not a node in list
    {
        return;
    }

    if (listNode->_nextListNode) 
    {
        listNode->_nextListNode->_prevListNode = listNode->_prevListNode;
    }
    if (listNode->_prevListNode) 
    {
        listNode->_prevListNode->_nextListNode = listNode->_nextListNode;
    }
    if (_head == listNode) 
    {
        _head = listNode->_nextListNode;
    }
    if (_tail == listNode) 
    {
        _tail = listNode->_prevListNode;
    }
    --_elementCount;
}

template<class T>
inline uint32_t  DoubleLinkList<T>::getElementCount() const 
{
    return _elementCount;
}

template<class T>
inline DoubleLinkListNode<T>* DoubleLinkList<T>::getTail() const
{
    return _tail;
}

template<class T>
inline DoubleLinkListNode<T>* DoubleLinkList<T>::getHead() const
{
    return _head;
}

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_DOUBLE_LINK_LIST_H
