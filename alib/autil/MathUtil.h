#ifndef AUTIL_MATHUTIL_H
#define AUTIL_MATHUTIL_H

#include "autil/CommonMacros.h"

AUTIL_BEGIN_NAMESPACE(autil);

// 100x faster than std::exp, std::pow
// 1.4 ~ 1.5% error compared with std::exp, std::pow
class MathUtil
{
public:
    static double exp(double x) {
        union {
            double d;
            int i[2];
        } u = {x};
        u.i[0] = 0;
        u.i[1] = (int)(1512775 * x + 1072632447);

        return u.d;
    }

    static double pow(double a, double b) {
        int e = (int) b;
        union {
            double d;
            int x[2];
        } u = { a };
        u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
        u.x[0] = 0;

        double r = 1.0;
        while (e) {
            if (e & 1) {
                r *= a;
            }
            a *= a;
            e >>= 1;
        }

        return r * u.d;
    }
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_MATHUTIL_H
