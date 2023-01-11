#ifndef FSLIB_METRICTAGSHANDLER_H
#define FSLIB_METRICTAGSHANDLER_H

#include <fslib/common/common_define.h>
#include <kmonitor/client/core/MetricsTags.h>

FSLIB_BEGIN_NAMESPACE(util);

class MetricTagsHandler
{
public:
    MetricTagsHandler();
    virtual ~MetricTagsHandler();
private:
    MetricTagsHandler(const MetricTagsHandler &);
    MetricTagsHandler& operator = (const MetricTagsHandler &);

public:
    virtual void getTags(const std::string& filePath, kmonitor::MetricsTags& tags) const;

private:
    static std::string PROXY_BEGIN;
    static std::string PROXY_SEP;
    static std::string PARAMS_SEP;
    static char INVAILD_CHAR;
    static char ESCAPE_CHAR;
};

FSLIB_TYPEDEF_SHARED_PTR(MetricTagsHandler);

FSLIB_END_NAMESPACE(util);

#endif //FSLIB_METRICTAGSHANDLER_H
