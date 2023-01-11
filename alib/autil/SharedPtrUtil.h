#ifndef AUTIL_SHAREDPTRUTIL_H
#define AUTIL_SHAREDPTRUTIL_H

#include "autil/CommonMacros.h"
#include <unistd.h>
#include <memory>

AUTIL_BEGIN_NAMESPACE(autil);

class SharedPtrUtil
{
private:
    SharedPtrUtil();
    ~SharedPtrUtil();
private:
    SharedPtrUtil(const SharedPtrUtil &);
    SharedPtrUtil& operator = (const SharedPtrUtil &);
public:
    template <class T>
    static bool waitUseCount(const std::shared_ptr<T> &ptr,
                             uint32_t expectUseCount = 1,
                             int64_t timeout = -1)
    {
        const int64_t interval = 10 * 1000;
        while (ptr.use_count() > expectUseCount) {
            if (timeout != -1) {
                if (timeout <= interval) {
                    return false;
                }
                timeout -= interval;
            }
            usleep(interval);
        }
        return true;
    }
private:

};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_SHAREDPTRUTIL_H
