#ifndef AUTIL_ATOMICCOUNTER_H
#define AUTIL_ATOMICCOUNTER_H

#include "autil/CommonMacros.h"

AUTIL_BEGIN_NAMESPACE(autil);

#define AUTIL_LOCK "lock ; "

class AtomicCounter
{
public:
    AtomicCounter() {
        _counter = 0;
    }

    inline int getValue() const
    {
        return _counter;
    }

    inline void setValue(int c)
    {
        _counter = c;
    }

    inline void add(int i)
    {
        asm volatile(
            AUTIL_LOCK "addl %1,%0"
            :"=m" (_counter)
            :"ir" (i), "m" (_counter));
    }

    inline void sub(int i)
    {
        asm volatile(
            AUTIL_LOCK "subl %1,%0"
            :"=m" (_counter)
            :"ir" (i), "m" (_counter));
    }

    inline int addAndReturn(int i)
    {
        int ret = i;
        asm volatile(
            AUTIL_LOCK "xaddl %0, %1"
            :"+r" (i), "+m" (_counter)
            : : "memory");
        return i + ret;
    }

    inline int subAndReturn(int i)
    {
        return addAndReturn(-i);
    }

    inline int subAndTest(int i)
    {
        unsigned char c;
        asm volatile(
            AUTIL_LOCK "subl %2,%0; sete %1"
            :"=m" (_counter), "=qm" (c)
            :"ir" (i), "m" (_counter) : "memory");
        return c;
    }

    inline void inc() {
        asm volatile(
            AUTIL_LOCK "incl %0"
            :"=m" (_counter)
            :"m" (_counter));
    }

    inline void dec() {
        asm volatile(
            AUTIL_LOCK "decl %0"
            :"=m" (_counter)
            :"m" (_counter));
    }

    inline bool incAndTest() {
        unsigned char c;
        asm volatile(
            AUTIL_LOCK "incl %0; sete %1"
            :"=m" (_counter), "=qm" (c)
            :"m" (_counter) : "memory");
        return c != 0;
    }

    inline bool decAndTest() {
        unsigned char c;
        asm volatile(
            AUTIL_LOCK "decl %0; sete %1"
            :"=m" (_counter), "=qm" (c)
            :"m" (_counter) : "memory");
        return c != 0;
    }

    inline int addNegative(int i) {
        unsigned char c;
        asm volatile(
            AUTIL_LOCK "addl %2,%0; sets %1"
            :"=m" (_counter), "=qm" (c)
            :"ir" (i), "m" (_counter) : "memory");
        return c;
    }

    inline void clearMask(int mask) {
        asm volatile(
            AUTIL_LOCK "andl %0,%1"
            : : "r" (~(mask)),"m" (_counter) : "memory");
    }

    inline void setMask(int mask) {
        asm volatile(
            AUTIL_LOCK "orl %0,%1"
            : : "r" (mask),"m" (_counter) : "memory");
    }

    inline int incAndReturn() {
        return addAndReturn(1);
    }

    inline int decAndReturn() {
        return subAndReturn(1);
    }

protected:
    volatile int _counter;
};

#undef AUTIL_LOCK
AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_ATOMICCOUNTER_H


