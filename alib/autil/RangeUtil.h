#ifndef AUTIL_RANGEUTIL_H
#define AUTIL_RANGEUTIL_H

#include <vector>
#include <cstdint>

namespace autil {

typedef std::pair<uint16_t, uint16_t> PartitionRange;
typedef std::vector<PartitionRange > RangeVec;

class RangeUtil
{
private:
    RangeUtil();
    ~RangeUtil();
private:
    RangeUtil(const RangeUtil &);
    RangeUtil& operator=(const RangeUtil &);
public:
    static RangeVec splitRange(uint32_t rangeFrom, uint32_t rangeTo,
                               uint32_t partitionCount);
private:
    static const uint32_t MAX_PARTITION_RANGE = 65535;
};

}

#endif //AUTIL_RANGEUTIL_H
