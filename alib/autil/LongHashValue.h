#ifndef AUTIL_LONGHASH_VALUE_H
#define AUTIL_LONGHASH_VALUE_H

#include "autil/CommonMacros.h"
#include <sstream>
#include <algorithm>
#include <assert.h>

AUTIL_BEGIN_NAMESPACE(autil);

template<int COUNT = 2>
class LongHashValue
{
public:
 
    LongHashValue(uint64_t integer) {
        for(int i = 0; i < COUNT - 1 ; i++) {
            value[i] = 0;
        }
        value[COUNT - 1] = integer;
    }
    
    LongHashValue() {
        for(int i = 0; i < COUNT ; i++) {
            value[i] = 0;
        }
    }

    size_t size() {
        return sizeof(value);
    }
    
    int count() {
        return COUNT;
    }
    
    bool operator ==(const LongHashValue<COUNT>& other) const
    {
        return std::equal(value, value + COUNT, other.value);
    }
    
    bool operator !=(const LongHashValue<COUNT>& other) const
    {
        return !(*this == other);
    }
    
    bool operator < (const LongHashValue<COUNT>& other) const
    {
        return std::lexicographical_compare(value, value + COUNT,
                other.value, other.value + COUNT);
    }
    
    bool operator > (const LongHashValue<COUNT>& other) const {
        return other < *this;
    }
    
    void serialize(std::string& str) const
    {
        str.resize(sizeof(uint64_t) * COUNT, '\0');
        
        uint64_t tmpPrimaryKey ;
        int len = sizeof(tmpPrimaryKey);
        for(int seg = 0; seg < COUNT; seg++)
        {
            tmpPrimaryKey = value[seg];
            for (int i = seg * len + len - 1; i >= seg * len; --i) 
            {
                str[i] = (char)(tmpPrimaryKey & 0xFF);
                tmpPrimaryKey >>= 8;
            }
        }
    }

    void deserialize(const std::string& str)
    {
        assert(str.length() == COUNT * sizeof(uint64_t));
        int len = sizeof(uint64_t);
        uint64_t tmpPrimaryKey;
        for(int seg = 0; seg < COUNT; seg++)
        {
            tmpPrimaryKey = (uint64_t)0;
            for(int i = seg * len; i < (seg+1) * len; i++)
            {
                tmpPrimaryKey <<= 8;
                tmpPrimaryKey |= (unsigned char)str[i];
                
            }
            value[seg] = tmpPrimaryKey;
        }
    }

    std::string toString()
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

public:
    uint64_t value[COUNT];
};

typedef LongHashValue<2> uint128_t;
typedef LongHashValue<4> uint256_t;

std::ostream& operator <<(std::ostream& stream, uint128_t v);
std::ostream& operator <<(std::ostream& stream, uint256_t v);

std::istream& operator >>(std::istream& stream, uint128_t& v);
std::istream& operator >>(std::istream& stream, uint256_t& v);

AUTIL_END_NAMESPACE(autil);
#endif //AUTIL_LONGHASHVALUE_H
