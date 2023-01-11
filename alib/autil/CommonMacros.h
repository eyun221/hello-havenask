#ifndef AUTIL_COMMON_MACROS_H
#define AUTIL_COMMON_MACROS_H
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#define AUTIL_BEGIN_NAMESPACE(x) namespace x {
#define AUTIL_END_NAMESPACE(x) }
#define AUTIL_USE_NAMESPACE(x) use namespace x

#define AUTIL_BEGIN_NAMESPACE_2(x, y) namespace x { namespace y {
#define AUTIL_END_NAMESPACE_2(x, y) } }
#define AUTIL_USE_NAMESPACE_2(x, y) use namespace x::y

#define DELETE_AND_SET_NULL(x) do {             \
        if ((x) != NULL) {                      \
            delete (x);                         \
            (x) = NULL;                         \
        }                                       \
    } while (false)

#define ARRAY_DELETE_AND_SET_NULL(x) do {       \
        if ((x) != NULL) {                      \
            delete[] (x);                       \
            (x) = NULL;                         \
        }                                       \
    } while (false)

#define likely(x) __builtin_expect((x), 1)

#define unlikely(x) __builtin_expect((x), 0)

//////////////////////////// copy from protobuf/stubs/common.h ///////////////////////
// The COMPILE_ASSERT macro can be used to verify that a compile time
// expression is true. For example, you could use it to verify the
// size of a static array:
//
//   COMPILE_ASSERT(ARRAYSIZE(content_type_names) == CONTENT_NUM_TYPES,
//                  content_type_names_incorrect_size);
//
// or to make sure a struct is smaller than a certain size:
// 
//   COMPILE_ASSERT(sizeof(foo) < 128, foo_too_large);
//
// The second argument to the macro is the name of the variable. If
// the expression is false, most compilers will issue a warning/error
// containing the name of the variable.

namespace autil {
template <bool>
struct CompileAssert {
};
}

#undef AUTIL_COMPILE_ASSERT
#define AUTIL_COMPILE_ASSERT(expr, msg)                                 \
    typedef ::autil::CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]

#endif //AUTIL_COMMON_MACROS_H
