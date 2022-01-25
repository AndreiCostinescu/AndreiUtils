//
// Created by andrei on 25.01.22.
//

#ifndef ANDREIUTILS_TIMEPOINT_HPP
#define ANDREIUTILS_TIMEPOINT_HPP

#include <AndreiUtils/enums/TimeUnit.h>
#include <AndreiUtils/utilsTime.h>

namespace AndreiUtils {
    class TimePoint : public SystemTimePoint {
    public:
        TimePoint();

        explicit TimePoint(const SystemTimePoint &timePoint);

        TimePoint &operator=(const SystemTimePoint &other);

        TimePoint addDelta(double time, const std::string &unit = "s");

        TimePoint addDelta(double time, TimeUnit unit);

        template<class TimePointTimeUnit>
        TimePoint addDelta(double time) {
            return TimePoint(
                    (*this) + std::chrono::duration_cast<TimePointTimeUnit>(std::chrono::duration<double>(time)));
        }

        SystemTimePoint getSystemTimePoint() const;
    };
}

#endif //ANDREIUTILS_TIMEPOINT_HPP
