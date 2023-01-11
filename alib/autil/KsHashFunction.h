#ifndef AUTIL_KSHASHFUNCTION_H
#define AUTIL_KSHASHFUNCTION_H

#include "autil/CommonMacros.h"
#include "autil/HashFunctionBase.h"

AUTIL_BEGIN_NAMESPACE(autil);

class KsHashFunction : public HashFunctionBase
{
public:
    KsHashFunction(const std::string& hashFunction, uint32_t partitionCout=65536, uint32_t ksHashRange = 720)
        : HashFunctionBase(hashFunction, partitionCout), _ksHashRange(ksHashRange)
    {}
    virtual ~KsHashFunction() {}

public:
    virtual uint32_t getHashId(const std::string& str) const;
    uint32_t getKsHashRange() const { return _ksHashRange; }
public:
    static uint32_t getHashId(uint64_t key, uint32_t rangeBefore, uint32_t rangeAfter);
    static uint32_t getHashId(uint32_t key, uint32_t rangeBefore, uint32_t rangeAfter);
private:
    uint32_t _ksHashRange;
};

typedef std::tr1::shared_ptr<KsHashFunction> KsHashFunctionPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_KSHASHFUNCTION_H
