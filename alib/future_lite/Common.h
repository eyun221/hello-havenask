#ifndef FUTURE_LITE_COMMON_H
#define FUTURE_LITE_COMMON_H

#include <future_lite/Log.h>
#include <stdexcept>

#define FL_LIKELY(x) __builtin_expect((x), 1)
#define FL_UNLIKELY(x) __builtin_expect((x), 0)

#define FL_INLINE __attribute__((__always_inline__)) inline

namespace future_lite {

inline void logicAssert(bool x, const char* errorMsg)
{
    if (FL_LIKELY(x)) {
        return;
    }
    throw std::logic_error(errorMsg);

}

}

#endif //FUTURE_LITE_COMMON_H
