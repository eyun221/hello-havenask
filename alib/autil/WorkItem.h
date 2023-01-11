#ifndef AUTIL_WORKITEM_H
#define AUTIL_WORKITEM_H

#include "autil/CommonMacros.h"

AUTIL_BEGIN_NAMESPACE(autil);

class WorkItem
{
public:
    WorkItem() {}
    virtual ~WorkItem() {}

public:
    virtual void process() = 0;
    virtual void destroy() = 0;
    virtual void drop() = 0;
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_WORKITEM_H
