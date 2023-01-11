/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-02-27 11:34
 * Author Name: xsank.mz
 * Author Email: xsank.mz@alibaba-inc.com
 * */

#ifndef KMONITOR_CLIENT_STATISTICSTYPE_H_
#define KMONITOR_CLIENT_STATISTICSTYPE_H_

#include "kmonitor/client/common/Common.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);

class StatisticsType {
 public:
    static bool NeedMaxMin(int flag);
    static bool NeedPercentile(int flag);
    static bool needSummary(int flag);

 private:
    StatisticsType(const StatisticsType &);
    StatisticsType &operator=(const StatisticsType &);

 public:
    static const int MIN = 1 << 1;
    static const int MAX = 1 << 2;
    static const int PERCENTILE_95 = 1 << 3;
    static const int PERCENTILE_99 = 1 << 4;
    static const int PERCENTILE_999 = 1 << 5;
    static const int SUM = 1 << 6;
    static const int SUMMARY = 1 << 7;
    static const int PERCENTILE_75 = 1 << 8;
    static const int COUNT = 1 << 9;
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_STATISTICSTYPE_H_
