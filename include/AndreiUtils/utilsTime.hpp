//
// Created by Andrei on 26-Jan-22.
//

#ifndef ANDREIUTILS_UTILSTIME_HPP
#define ANDREIUTILS_UTILSTIME_HPP

#include <AndreiUtils/utilsTime.h>

namespace AndreiUtils {
    template<class TimeUnitRatio>
    SystemTimePoint addDeltaTime(const SystemTimePoint &timePoint, double deltaT) {
        return timePoint + std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::duration<double, TimeUnitRatio>(deltaT));
    }

    template<class TimeUnitRatio>
    SystemTimePoint getTimePoint(double t) {
        return SystemTimePoint(
                std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double, TimeUnitRatio>(t)));
    }
}

#endif //ANDREIUTILS_UTILSTIME_HPP
