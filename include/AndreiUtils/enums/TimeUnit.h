//
// Created by Andrei on 25.01.22.
//

#ifndef ANDREIUTILS_TIMEUNITS_H
#define ANDREIUTILS_TIMEUNITS_H

#include <string>

namespace AndreiUtils {
    enum TimeUnit {
        DAY,
        HOUR,
        MINUTE,
        SECOND,
        MILLISECOND,
        MICROSECOND,
        NANOSECOND,
    };

    double getMultiplicationFactorRelativeToSeconds(TimeUnit timeUnit);

    std::string convertTimeUnitToString(const TimeUnit &timeUnit);

    TimeUnit convertStringToTimeUnit(const std::string &timeUnit);
}

#endif //ANDREIUTILS_TIMEUNITS_H
