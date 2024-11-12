//
// Created by Andrei on 16.08.24.
//

#include <AndreiUtils/utilsLimits.h>

double const AndreiUtils::DOUBLE_INF = std::numeric_limits<double>::max() / 2;
double const AndreiUtils::DOUBLE_INF_NEG = -DOUBLE_INF;
long const AndreiUtils::LONG_INF = std::numeric_limits<long>::max() / 2 + 1;  // 31 / 2 = 15; + 1 = 16;
long const AndreiUtils::LONG_INF_NEG = std::numeric_limits<long>::min() / 2;