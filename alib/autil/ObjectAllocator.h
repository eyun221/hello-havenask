#ifndef AUTIL_OBJECTALLOCATOR_H
#define AUTIL_OBJECTALLOCATOR_H

#include "autil/CommonMacros.h"

#include "autil/FixedSizeAllocator.h"

AUTIL_BEGIN_NAMESPACE(autil);

template<typename T>
class ObjectAllocator
{
public:
    ObjectAllocator() 
        : allocator(sizeof(T))
    {
    }
    ~ObjectAllocator() {
    }
public:
    T* allocate() {
        return new (allocator.allocate()) T();
    }
    void free(T* t) {
        t->~T();
        allocator.free((void *)t);
    }
    size_t getCount() const {return allocator.getCount();}
private:
    FixedSizeAllocator allocator;
private:
    friend class ObjectAllocatorTest;
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_OBJECTALLOCATOR_H
