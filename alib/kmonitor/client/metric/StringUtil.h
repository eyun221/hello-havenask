/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2019-07-24 10:47
 * Author Name: yixuan.ly
 * Author Email: yixuan.ly@alibaba-inc.com
 */
#ifndef KMONITOR_CLIENT_METRIC_STRINGUTIL_H
#define KMONITOR_CLIENT_METRIC_STRINGUTIL_H

#include "kmonitor/client/common/Common.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);

class StringUtil
{
public:
    StringUtil();
    ~StringUtil();
private:
    StringUtil(const StringUtil &);
    StringUtil& operator = (const StringUtil &);
public:
    static const char* skipToken(const char *str);
    static const char* skipWhite(const char *str);
};

TYPEDEF_PTR(StringUtil);

END_KMONITOR_NAMESPACE(kmonitor);

#endif //KMONITOR_CLIENT_METRIC_STRINGUTIL_H
