#ifndef AUTIL_DAILYRUNMODE_H
#define AUTIL_DAILYRUNMODE_H

#include "autil/CommonMacros.h"
#include <stdlib.h>
#include <string>

AUTIL_BEGIN_NAMESPACE(autil);

class DailyRunMode
{
private:
    DailyRunMode();
    ~DailyRunMode();
public:
    static bool enable();
public:
    static const std::string DAILY_RUN_MODE;
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_DAILYRUNMODE_H
