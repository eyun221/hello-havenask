#ifndef AUTIL_ALLOCATORS_H
#define AUTIL_ALLOCATORS_H

#include "autil/CommonMacros.h"
#include <sys/mman.h>
#include <assert.h>

AUTIL_BEGIN_NAMESPACE(autil);

struct SysAllocator
{
    void* allocate(size_t nBytes) {
        return ::malloc(nBytes);
    }
    void deallocate(void *ptr, size_t /*nBytes*/) {
        ::free(ptr);
    }
};

struct MmapAllocator
{
    void* allocate(size_t nBytes) {
        void *ret = mmap(NULL, nBytes, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        return (ret == MAP_FAILED) ? NULL : ret;
    }
    void deallocate(void *ptr, size_t nBytes) {
        int ret = munmap(ptr, nBytes);
        (void) ret; assert(ret == 0);
    }
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_ALLOCATORS_H
