/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2017-07-26 17:33
 * Author Name: beifei
 * Author Email: beifei@taobao.com
 */

#ifndef KMONITOR_CLIENT_COMMON_MINMAXCALCULATOR_H_
#define KMONITOR_CLIENT_COMMON_MINMAXCALCULATOR_H_

#include <limits>
#include "kmonitor/client/common/Common.h"
BEGIN_KMONITOR_NAMESPACE(kmonitor);

class MinMaxCalculator {
 public:
    MinMaxCalculator();
    ~MinMaxCalculator();

 public:
    void Add(double value);
    double Min() const;
    double Max() const;
    void Reset();


 private:
    MinMaxCalculator(const MinMaxCalculator &);
    MinMaxCalculator& operator=(const MinMaxCalculator &);

 private:
    double min_;
    double max_;
};

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_COMMON_MINMAXCALCULATOR_H_
