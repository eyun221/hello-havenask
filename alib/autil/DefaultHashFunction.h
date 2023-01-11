#ifndef AUTIL_DEFAULTHASHFUNCTION_H
#define AUTIL_DEFAULTHASHFUNCTION_H

#include "autil/CommonMacros.h"
#include "autil/HashFunctionBase.h"
AUTIL_BEGIN_NAMESPACE(autil);

class DefaultHashFunction : public HashFunctionBase
{
public:
    DefaultHashFunction(const std::string& hashFunction, uint32_t partitionCount)
        : HashFunctionBase(hashFunction, partitionCount) {}
    virtual ~DefaultHashFunction(){}
public:
    /*override*/virtual uint32_t getHashId(const std::string& str) const;
    /* override */ uint32_t getHashId(const char *buf, size_t len) const;
public:
    int hashString(const std::string& str) const;
};

typedef std::tr1::shared_ptr<DefaultHashFunction> DefaultHashFunctionPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_DEFAULTHASHFUNCTION_H
