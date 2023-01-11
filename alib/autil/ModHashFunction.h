#ifndef AUTIL_MODHASHFUNCTION_H
#define AUTIL_MODHASHFUNCTION_H

#include "autil/CommonMacros.h"
#include "autil/HashFunctionBase.h"
AUTIL_BEGIN_NAMESPACE(autil);

class ModHashFunction : public HashFunctionBase
{
public:
    ModHashFunction(const std::string& hashFunction, uint32_t partitionCount)
        : HashFunctionBase(hashFunction, partitionCount) {}
    virtual ~ModHashFunction(){}
public:
    /*override*/virtual uint32_t getHashId(const std::string& str) const;
};

typedef std::tr1::shared_ptr<ModHashFunction> ModHashFunctionPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_ModHASHFUNCTION_H
