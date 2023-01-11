#ifndef AUTIL_RAPIDJSON_AUTIL_RAPIDJSON_COMMON_H_
#define AUTIL_RAPIDJSON_AUTIL_RAPIDJSON_COMMON_H_

#include <autil/mem_pool/Pool.h>

#ifndef RAPIDJSON_HAS_STDSTRING
#define RAPIDJSON_HAS_STDSTRING 1
#endif

#include "autil/legacy/rapidjson/rapidjson.h"
#include "autil/legacy/rapidjson/document.h"
#include "autil/legacy/rapidjson/writer.h"
#include "autil/legacy/rapidjson/stringbuffer.h"
#include "autil/legacy/rapidjson/error/en.h"

AUTIL_RAPIDJSON_NAMESPACE_BEGIN

class AutilPoolAllocator {
public:
    static const bool kNeedFree = false;

    AutilPoolAllocator(autil::mem_pool::Pool *pool_ = NULL)
    {
        if (pool_) {
            pool = pool_;
            ownPool = false;
        } else {
            pool = new autil::mem_pool::Pool;
            ownPool = true;
        }
    }
    ~AutilPoolAllocator() {
        if (ownPool) {
            DELETE_AND_SET_NULL(pool);
        }
    }
private:
    AutilPoolAllocator(const AutilPoolAllocator &);
    AutilPoolAllocator& operator=(const AutilPoolAllocator &);

public:
    void* Malloc(size_t size) {
        if (size == 0) {
            return NULL;
        } else {
            return pool->allocate(size);
        }
    }
    void* Realloc(void* originalPtr, size_t originalSize, size_t newSize) {
        // do nothing to old mem
        if (originalPtr == 0) {
            return Malloc(newSize);
        }
        if (newSize == 0) {
            return NULL;
        }
        if (originalSize >= newSize) {
            return originalPtr;
        }
        void* newBuffer = Malloc(newSize);
        assert(newBuffer != 0);
        if (originalSize)
            std::memcpy(newBuffer, originalPtr, originalSize);
        return newBuffer;
    }
    static void Free(void *ptr) {
        // do nothing to old mem
    }
public:
    autil::mem_pool::Pool *pool;
    bool ownPool;
};

typedef GenericValue<ASCII<>, AutilPoolAllocator> SimpleValue;
typedef GenericDocument<ASCII<>, AutilPoolAllocator> SimpleDocument;

AUTIL_RAPIDJSON_NAMESPACE_END

#endif //AUTIL_RAPIDJSON_AUTIL_RAPIDJSON_COMMON_H_
