#ifndef AUTIL_TIMEUTILITY_H
#define AUTIL_TIMEUTILITY_H

#include "autil/CommonMacros.h"
#include <sys/time.h>
#include <string>

AUTIL_BEGIN_NAMESPACE(autil);

class TimeUtility 
{
public:
    static int64_t currentTime();
    static int64_t currentTimeInSeconds();
    static int64_t currentTimeInMicroSeconds();
    static int64_t currentTimeInNanoSeconds();
    static int64_t getTime(int64_t usecOffset = 0);
    static timeval getTimeval(int64_t usecOffset = 0);
    static timespec getTimespec(int64_t usecOffset = 0);
    static int64_t us2ms(int64_t usecond) {
        return usecond / 1000;
    }
    static std::string currentTimeString();
    static std::string currentTimeString(const std::string &format);
};

class ScopedTime {
public:
    ScopedTime(int64_t &t) : _t(t) {
        _b = TimeUtility::currentTime();
    }
    ~ScopedTime() {
        _t += TimeUtility::currentTime() - _b;
    }
private:
    int64_t &_t;
    int64_t _b;
};

AUTIL_END_NAMESPACE(autil);

#endif // AUTIL_TIMEUTILITY_H
