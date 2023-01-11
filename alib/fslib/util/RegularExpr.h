#ifndef FSLIB_REGULAREXPR_H
#define FSLIB_REGULAREXPR_H

#include <autil/Log.h>
#include "fslib/fslib.h"
#include <sys/types.h>
#include <regex.h>

FSLIB_BEGIN_NAMESPACE(util);

class RegularExpr
{
public:
    RegularExpr();
    ~RegularExpr();
    
public:
    bool init(const std::string &pattern);
    bool match(const std::string &string) const;

private:
    regex_t _regex;
    bool _init;
};

FSLIB_TYPEDEF_SHARED_PTR(RegularExpr);
typedef std::vector<RegularExprPtr> RegularExprVector;

FSLIB_END_NAMESPACE(util);

#endif //FSLIB_REGULAREXPR_H
