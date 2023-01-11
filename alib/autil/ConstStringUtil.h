#pragma once

#include "autil/CommonMacros.h"
#include "autil/ConstString.h"
#include "autil/StringUtil.h"
#include "autil/MultiValueType.h"
#include "autil/MultiValueCreator.h"
#include <limits>
#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

AUTIL_BEGIN_NAMESPACE(autil);

class ConstStringUtil
{
public:
    template<typename T>
    static bool fromString(const autil::ConstString *value, T &val, autil::mem_pool::Pool *pool);

    template <typename T>
    static bool fromString(const autil::ConstString *value,
                           autil::MultiValueType<T> &val,
                           autil::mem_pool::Pool *pool);
};

template <typename T>
inline bool ConstStringUtil::fromString(const autil::ConstString *value,
                                        T &val,
                                        autil::mem_pool::Pool *pool)
{
    assert(value);
    return autil::StringUtil::fromString<T>(value->toString(), val);
}

template <typename T>
inline bool ConstStringUtil::fromString(const autil::ConstString *value,
                                        autil::MultiValueType<T> &val,
                                        autil::mem_pool::Pool *pool)
{
    assert(value);
    std::vector<T> valVec;
    autil::StringUtil::fromString(
            value->toString(), valVec, std::string(1, autil::MULTI_VALUE_DELIMITER));
    char *buffer = autil::MultiValueCreator::createMultiValueBuffer<T>(valVec, pool);
    if (buffer == nullptr) {
        return false;
    }
    val.init(buffer);
    return true;
}

template <>
inline bool ConstStringUtil::fromString<char>(
        const autil::ConstString *value,
        autil::MultiChar &val,
        autil::mem_pool::Pool *pool)
{
    assert(value);
    char *buffer = autil::MultiValueCreator::createMultiValueBuffer(
            value->data(), value->size(), pool);
    if (buffer == nullptr) {
        return false;
    }
    val.init(buffer);
    return true;
}

template <>
inline bool ConstStringUtil::fromString<autil::MultiChar>(
        const autil::ConstString *value,
        autil::MultiString &val,
        autil::mem_pool::Pool *pool)
{
    assert(value);
    std::vector<std::string> strVec;
    autil::StringUtil::split(
            strVec, value->toString(), autil::MULTI_VALUE_DELIMITER);
    char *buffer = autil::MultiValueCreator::createMultiValueBuffer(strVec, pool);
    if (buffer == nullptr) {
        return false;
    }
    val.init(buffer);
    return true;
}

AUTIL_END_NAMESPACE(autil);
