#ifndef AUTIL_URLUTIL_H
#define AUTIL_URLUTIL_H
#include "autil/CommonMacros.h"

#include <string>

AUTIL_BEGIN_NAMESPACE(autil);

class URLUtil
{
public:
    URLUtil();
    ~URLUtil();
public:
    static std::string decode(const std::string &str);
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_URLUTIL_H
