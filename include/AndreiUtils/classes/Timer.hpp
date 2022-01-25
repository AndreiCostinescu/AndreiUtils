//
// Created by Andrei on 13.09.2021.
//

#ifndef ANDREIUTILS_TIMER_HPP
#define ANDREIUTILS_TIMER_HPP

#include <AndreiUtils/enums/TimeUnit.h>
#include <chrono>
#include <string>

namespace AndreiUtils {
    class Timer {
    public:
        explicit Timer(bool startTiming = true);

        void start();

        void reset();

        double measure(const std::string &timeUnit = "s") const;

        double measure(TimeUnit timeUnit) const;

        template<class TimerTimeUnit>
        double measure() {
            return std::chrono::duration_cast<TimerTimeUnit>(clockType::now() - this->startTime).count();
        }

    private:
        typedef std::chrono::high_resolution_clock clockType;
        typedef std::chrono::time_point<clockType> timePoint;

        timePoint startTime;
    };
}

#endif //ANDREIUTILS_TIMER_HPP
