#ifndef AUTIL_FIXEDSIZEALLOCATOR_H
#define AUTIL_FIXEDSIZEALLOCATOR_H

#include "autil/CommonMacros.h"
#include <vector>
#include <stdlib.h>
#include <cassert>
#include <tr1/memory>

AUTIL_BEGIN_NAMESPACE(autil);

class FixedSizeAllocator
{
public:
    static const size_t COUNT_PER_BLOCK = 4 * 1024;
public:
    FixedSizeAllocator(size_t fixedSize) {
        _fixedSize = fixedSize > sizeof(void *) ? fixedSize : sizeof(void *);
        _start = NULL;
        _free = NULL;
        _end = NULL;
        _freeList = NULL;
        _count = 0;
        _bufVec.reserve(1024);
    }
    ~FixedSizeAllocator() {
        assert(_count == 0);
        clear();
    }
public:
    void* allocate() {
        void *ret;
        _count++;
        if (_freeList) {
            ret = _freeList;
            _freeList  = *static_cast<void **>(_freeList);
        } else if (_free < _end) {
            ret = _free;
            _free += _fixedSize;
        } else {
            _start = (char *)(::malloc(COUNT_PER_BLOCK * _fixedSize));
            if (!_start) {
                _count--;
                return NULL;
            }
            _end = _start + COUNT_PER_BLOCK * _fixedSize;
            _free = _start + _fixedSize;
            _bufVec.push_back(_start);
            ret = _start;
        } 
        
        return ret;
    }
    void free(void* t) {
        _count--;
        *reinterpret_cast<void **>(t) = _freeList;
        _freeList = t;
    }
    size_t getCount() const {return _count;}

    void clear() {
        for (std::vector<char *>::const_iterator it = _bufVec.begin();
             it != _bufVec.end(); ++it)
        {
            ::free(*it);
        }
        _bufVec.clear();

        _start = NULL;
        _free = NULL;
        _end = NULL;
        _freeList = NULL;
        _count = 0;
    }

private:
    size_t _fixedSize;
    char *_start;
    char *_free;
    char *_end;
    void *_freeList;
    size_t _count;
    std::vector<char *> _bufVec;
private:
    friend class FixedSizeAllocatorTest;
};

typedef std::tr1::shared_ptr<FixedSizeAllocator> FixedSizeAllocatorPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_FIXEDSIZEALLOCATOR_H
