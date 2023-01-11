#ifndef AUTIL_HASHALGORITHM_H
#define AUTIL_HASHALGORITHM_H

#include "autil/CommonMacros.h"
#include "autil/LongHashValue.h"
AUTIL_BEGIN_NAMESPACE(autil);

class HashAlgorithm
{
public:
    HashAlgorithm();
    ~HashAlgorithm();
public:
    static uint32_t hashString(const char *str, uint32_t dwHashType);
    static uint32_t hashString(const char *str, size_t size, uint32_t dwHashType);
    static uint64_t hashString64(const char *str);
    static uint64_t hashString64(const char *str, size_t size);
    static uint64_t hashNumber64(int32_t number);
    static uint128_t hashString128(const char* str);
    static uint128_t hashString128(const char* str, size_t size);
public:
    //deprecated hash function, for it not produces hash value unifromly
    static uint64_t hashString64Deprecated(const char *str, uint32_t dwHashType);
    static uint64_t hashString64Deprecated(const char *str, size_t size, uint32_t dwHashType);
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_HASHALGORITHM_H
