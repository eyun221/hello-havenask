#ifndef AUTIL_HASH64FUNCTION_H
#define AUTIL_HASH64FUNCTION_H

#include "autil/CommonMacros.h"
#include "autil/HashFunctionBase.h"
AUTIL_BEGIN_NAMESPACE(autil);

class Hash64Function : public HashFunctionBase
{
public:
    Hash64Function(const std::string& hashFunction, uint32_t partitionCount)
        : HashFunctionBase(hashFunction, partitionCount) {}
    virtual ~Hash64Function(){}
public:
    /*override*/virtual uint32_t getHashId(const std::string& str) const;
    /* override */ uint32_t getHashId(const char *buf, size_t len) const;
};

typedef std::tr1::shared_ptr<Hash64Function> Hash64FunctionPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_HASH64FUNCTION_H
