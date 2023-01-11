#ifndef AUTIL_NUMBERHASHFUNCTION_H
#define AUTIL_NUMBERHASHFUNCTION_H

#include "autil/CommonMacros.h"
#include "autil/HashFunctionBase.h"
AUTIL_BEGIN_NAMESPACE(autil);

class NumberHashFunction : public HashFunctionBase
{
public:
    NumberHashFunction(const std::string& hashFunction, uint32_t partitionCount)
        :HashFunctionBase(hashFunction, partitionCount) {}
    virtual ~NumberHashFunction() {}
public:
    /*override*/virtual uint32_t getHashId(const std::string& str) const;
public:
    int64_t hashString(const std::string& str) const;
};

typedef std::tr1::shared_ptr<NumberHashFunction> NumberHashFunctionPtr;
AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_NUMBERHASHFUNCTION_H
