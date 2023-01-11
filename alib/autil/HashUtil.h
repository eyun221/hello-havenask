#ifndef AUTIL_HASHUTIL_H
#define AUTIL_HASHUTIL_H

#include "autil/CommonMacros.h"
#include "autil/HashAlgorithm.h"
#include "autil/LongHashValue.h"

AUTIL_BEGIN_NAMESPACE(autil);

template <class T>
class KeyHash {
};

template<>
class KeyHash<char>
{
public:
    inline uint64_t operator()(char key) const
    {
        return (uint64_t)key;
    }
};

template<>
class KeyHash<const char*>
{
public:
    uint64_t operator()(const char* key) const
    {
        return HashAlgorithm::hashString64(key);
    }
};

template<>
class KeyHash<std::string>
{
public:
    uint64_t operator()(const std::string& key) const
    {
        return HashAlgorithm::hashString64(key.c_str());
    }
};

template<>
class KeyHash<uint64_t>
{
public:
    uint64_t operator()(uint64_t key) const
    {
        return key;
    }
};

template<>
class KeyHash<int64_t>
{
public:
    uint64_t operator()(int64_t key) const
    {
        return (uint64_t)key;
    }
};

template<>
class KeyHash<uint32_t>
{
public:
    uint64_t operator()(uint32_t key) const
    {
        return (uint64_t)key;
    }
};

template<>
class KeyHash<int32_t>
{
public:
    uint64_t operator()(int32_t key) const
    {
        return (uint64_t)key;
    }
};

template<>
class KeyHash<uint16_t>
{
public:
    uint64_t operator()(uint16_t key) const
    {
        return (uint64_t)key;
    }
};

template<>
class KeyHash<int16_t>
{
public:
    uint64_t operator()(int16_t key) const
    {
        return (uint64_t)key;
    }
};

template<>
class KeyHash<uint8_t>
{
public:
    uint64_t operator()(uint8_t key) const
    {
        return (uint64_t)key;
    }
};

template<>
class KeyHash<int8_t>
{
public:
    uint64_t operator()(int8_t key) const
    {
        return (uint64_t)key;
    }
};

template<>
class KeyHash<float>
{
public:
    uint64_t operator()(float key) const
    {
        union {float k; uint64_t i;} a;
        a.k = a.i = 0;
        a.k = key;
        return a.i;
    }
};

template<>
class KeyHash<uint128_t>
{
public:
    uint64_t operator()(uint128_t key) const
    {
        return key.value[0];   
    }
};

template <typename Key>
class KeyEqual {
};

template<>
class KeyEqual<char> 
{
public:
    inline bool operator()(char cKey1, char cKey2) const 
    {
        return cKey1 == cKey2;
    }
};

template<> 
class KeyEqual<const char*> 
{
public:
    inline bool operator()(const char* szKey1, const char* szKey2) const 
    { 
        return strcmp(szKey1,szKey2) == 0;
    }
};

template<> 
class KeyEqual<std::string> 
{
public:
    inline bool operator()(const std::string& key1, const std::string& key2) const 
    { 
        return key1 == key2;
    }
};

template<> 
class KeyEqual<uint8_t> 
{
public:
    inline bool operator()(uint8_t nKey1, uint8_t nKey2) const 
    { 
        return nKey1 == nKey2;
    }
};

template<> 
class KeyEqual<int8_t> 
{
public:
    inline bool operator()(int8_t nKey1, int8_t nKey2) const 
    { 
        return nKey1 == nKey2;
    }
};

template<> 
class KeyEqual<uint16_t> 
{
public:
    inline bool operator()(uint16_t nKey1, uint16_t nKey2) const 
    { 
        return nKey1 == nKey2;
    }
};

template<> 
class KeyEqual<int16_t> 
{
public:
    inline bool operator()(int16_t nKey1, int16_t nKey2) const 
    { 
        return nKey1 == nKey2;
    }
};

template<> 
class KeyEqual<uint32_t> 
{
public:
    inline bool operator()(uint32_t nKey1, uint32_t nKey2) const 
    { 
        return nKey1 == nKey2;
    }
};

template<> 
class KeyEqual<int32_t> 
{
public:
    inline bool operator()(int32_t nKey1, int32_t nKey2) const 
    { 
        return nKey1 == nKey2;
    }
};

template<> 
class KeyEqual<uint64_t> 
{
public:
    inline bool operator()(uint64_t nKey1, uint64_t nKey2) const 
    { 
        return nKey1 == nKey2;
    }
};

template<> 
class KeyEqual<int64_t> 
{
public:
    inline bool operator()(int64_t nKey1, int64_t nKey2) const 
    { 
        return nKey1 == nKey2;
    }
};

template<> 
class KeyEqual<float> 
{
public:
    inline bool operator()(float nKey1, float nKey2) const 
    { 
        return nKey1 == nKey2;
    }
};

template<> 
class KeyEqual<uint128_t> 
{
public:
    inline bool operator()(uint128_t nKey1, uint128_t nKey2) const 
    { 
        return nKey1 == nKey2;
    }
};

template <typename Key>
class KeyCompare 
{
};

template<> 
class KeyCompare<const char*> 
{
public:
    inline bool operator()(const char* szKey1, const char* szKey2) const 
    { 
        return strcmp(szKey1,szKey2) < 0;
    }
};

template<> 
class KeyCompare<uint32_t> 
{
public:
    inline bool operator()(uint32_t nKey1, uint32_t nKey2) const 
    { 
        return nKey1 < nKey2;
    }
};

template<> 
class KeyCompare<int32_t> 
{
public:
    inline bool operator()(int32_t nKey1, int32_t nKey2) const 
    { 
        return nKey1 < nKey2;
    }
};

template<> 
class KeyCompare<float> 
{
public:
    inline bool operator()(float nKey1, float nKey2) const 
    { 
        return nKey1 < nKey2;
    }
};

template<> 
class KeyCompare<uint64_t> 
{
public:
    inline bool operator()(uint64_t nKey1, uint64_t nKey2) const 
    { 
        return nKey1 < nKey2;
    }
};

template<> 
class KeyCompare<int64_t> 
{
public:
    inline bool operator()(int64_t nKey1, int64_t nKey2) const 
    { 
        return nKey1 < nKey2;
    }
};

template<> 
class KeyCompare<uint128_t> 
{
public:
    inline bool operator()(uint128_t nKey1, uint128_t nKey2) const 
    { 
        if((nKey1.value[0] < nKey2.value[0]))
            return true;
         if((nKey1.value[0] > nKey2.value[0]))
             return false;
         return (nKey1.value[1] < nKey2.value[1]);
    }
};

template < typename Type, typename Compare = KeyCompare<Type> >
class BinarySearch
{
public:
    inline uint32_t operator()(Type *szElements, uint32_t nBegin,
                               uint32_t nEnd, Type value)
    {
        return std::lower_bound(szElements + nBegin,
                                szElements + nEnd, value, Compare()) - szElements;
    }
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_HASHUTIL_H
