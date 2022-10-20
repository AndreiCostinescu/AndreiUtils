//
// Created by Andrei on 07-Mar-22.
//

#ifndef ANDREIUTILS_UTILSGEOMETRY_H
#define ANDREIUTILS_UTILSGEOMETRY_H

#include <cmath>

namespace AndreiUtils {
    extern long double const DEG2RAD_FACTOR;
    extern long double const RAD2DEG_FACTOR;

    float deg2Rad(float x);

    double deg2Rad(double x);

    long double deg2Rad(long double x);

    float rad2Deg(float x);

    double rad2Deg(double x);

    long double rad2Deg(long double x);

    bool inContact(const float (&p1)[3], const float (&p2)[3], double threshold);

    bool inContact(const float (&p1)[3], const float (&p2)[3], double threshold, double &distance);
}

#endif //ANDREIUTILS_UTILSGEOMETRY_H
