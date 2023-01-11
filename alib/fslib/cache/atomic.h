#ifndef KINGSO_CACHE_ATOMIC_H
#define KINGSO_CACHE_ATOMIC_H
#include <atomic>

FSLIB_BEGIN_NAMESPACE(cache);

typedef std::atomic_long atomic64_t;

#define atomic64_read(v)    ((v)->load())
#define atomic64_set(v,i)   ((v)->store((i)))

static __inline__ void atomic64_add(long i, atomic64_t *v)
{
    atomic64_t &tmp = *v;
    tmp += i;
}

static __inline__ void atomic64_sub(long i, atomic64_t *v)
{
    atomic64_t &tmp = *v;
    tmp -= i;
}

FSLIB_END_NAMESPACE(cache);
#endif
