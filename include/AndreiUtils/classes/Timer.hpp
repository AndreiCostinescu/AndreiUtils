//
// Created by Andrei on 13.09.2021.
//

#ifndef ANDREIUTILS_TIMER_HPP
#define ANDREIUTILS_TIMER_HPP

#include <chrono>
#include <string>

namespace AndreiUtils {
    class Timer {
    public:
        enum TimerTimeUnit {
            DAY,
            HOUR,
            MINUTE,
            SECOND,
            MILLISECOND,
            MICROSECOND,
            NANOSECOND,
        };

        explicit Timer(bool startTiming = true);

        void start();

        void reset();

        double measure(const std::string &timeUnit = "s") const;

        double measure(TimerTimeUnit timeUnit) const;

        template<class TimeUnit>
        double measure() {
            return std::chrono::duration_cast<TimeUnit>(clockType::now() - this->startTime).count();
        }

    private:
        typedef std::chrono::high_resolution_clock clockType;
        typedef std::chrono::time_point<clockType> timePoint;

        static double getMultiplicationFactor(TimerTimeUnit timeUnit);

        timePoint startTime;
    };
}

#endif //ANDREIUTILS_TIMER_HPP
