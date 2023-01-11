#ifndef FSLIB_URLPARSER_H
#define FSLIB_URLPARSER_H

#include <tuple>
#include <fslib/fslib.h>
#include "autil/ConstString.h"

FSLIB_BEGIN_NAMESPACE(util);

class URLParser
{
public:
    using Slice = autil::ConstString;

    // rawPath is parameterized, such as, pangu://ea120cloudpangu?app=0321^BIGFILE_APPNAME/root/
    // purePath is removed parameters, such as, pangu://ea120cloudpangu/root/
    // configStr is parameters, such as, app=0321^BIGFILE_APPNAME
    static std::string getPurePath(const std::string& rawPath, std::string* paramsStr = NULL);
    static std::string appendTrailingSlash(const std::string& dirPath);
    static std::string trimTrailingSlash(const std::string& path);
    static std::string normalizePath(const std::string& path);

    // <scheme>://<netloc>?<params><path> into [scheme, netloc, params, path]
    static std::tuple<Slice, Slice, Slice, Slice> splitPath(const std::string& rawPath);

    // overwrite=true will rm all key in rawPath first, otherwise append only
    static std::string appendParam(const std::string& rawPath, const std::string& key,
                                   const std::string& value, bool overwrite = false);
    static std::string eraseParam(const std::string& rawPath, const std::string& key);

private:
    static std::string assemblePath(const Slice& scheme, const Slice& netloc,
                                    const Slice& params, const Slice& path);
public:
    static const constexpr char* PARAMS_CONFIG_SEPARATOR = "?";
    static const constexpr char* PARAMS_KV_SEPARATOR = "=";
    static const constexpr char* PARAMS_KV_PAIR_SEPARATOR = "&";
    static const constexpr char* NETLOC_SEPARATOR = "://";
};

FSLIB_END_NAMESPACE(util);

#endif // FSLIB_URLPARSER_H
