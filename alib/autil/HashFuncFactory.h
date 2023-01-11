#ifndef AUTIL_HASHFUNCFACTORY_H
#define AUTIL_HASHFUNCFACTORY_H

#include "autil/CommonMacros.h"
#include "autil/HashFunctionBase.h"

AUTIL_BEGIN_NAMESPACE(autil);

class HashFuncFactory
{
public:
    HashFuncFactory();
    ~HashFuncFactory();
public:
    static HashFunctionBasePtr createHashFunc(const std::string& funcName, 
            uint32_t partitionCount = 65536);
    static HashFunctionBasePtr createHashFunc(const std::string& funcName, 
            const std::map<std::string, std::string> &params,
            uint32_t partitionCount = 65536);

};

typedef std::tr1::shared_ptr<HashFuncFactory> HashFuncFactoryPtr;


AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_HASHFUNCFACTORY_H
