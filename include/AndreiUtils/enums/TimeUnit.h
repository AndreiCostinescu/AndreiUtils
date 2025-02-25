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

    [[nodiscard]] TimeUnit getLargestTimeUnit();

    [[nodiscard]] TimeUnit getSmallestTimeUnit();

    [[nodiscard]] double getMultiplicationFactorRelativeToSeconds(TimeUnit timeUnit);

    [[nodiscard]] std::string convertTimeUnitToString(TimeUnit const &timeUnit);

    [[nodiscard]] TimeUnit convertStringToTimeUnit(std::string const &timeUnit);
}

#endif //ANDREIUTILS_TIMEUNITS_H
