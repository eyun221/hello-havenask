#ifndef ANET_UTIL_H
#define ANET_UTIL_H

#include <anet/timeutil.h>
#include <stdlib.h>

namespace anet {

inline int getRandInt(int max)
{
    int seed = (int)TimeUtil::getTime();
    srand(seed);
    int rc = rand() % max;
    return rc;
}

inline uint32_t revertBit(uint32_t num, int pos)
{
    uint32_t o = ~num;
    pos = pos % 32;
    int n = 1 << pos;

    return((num & ~n) | (o & n));
}

}

#endif
