#ifndef __AUTIL_ENV_UTIL_H
#define __AUTIL_BLOCK_H

#include <memory>
#include "autil/StringUtil.h"
#include <stdlib.h>

AUTIL_BEGIN_NAMESPACE(autil)

class EnvUtil {
public:
    EnvUtil() = delete;

public:
    template <typename T>
    static T getEnv(const std::string& key, const T& defaulValue) {
        const char* str = getenv(key.c_str());
        if (!str) {
            return defaulValue;
        }
        T ret = T();
        auto success = autil::StringUtil::fromString(str, ret);
        if (success) {
            return ret;
        } else {
            return defaulValue;
        }
    }

    static int setEnv(const std::string& env, const std::string& value) {
        return setenv(env.c_str(), value.c_str(), 1);
    }

    static int unsetEnv(const std::string& env) { return unsetenv(env.c_str()); }
};


AUTIL_END_NAMESPACE(autil)

#endif
