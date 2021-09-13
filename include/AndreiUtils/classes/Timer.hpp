//
// Created by ga78cat on 13.09.2021.
//

#ifndef ANDREIUTILS_TIMER_HPP
#define ANDREIUTILS_TIMER_HPP

#include <chrono>

namespace AndreiUtils {
    template<class TimeUnit>
    class Timer {
    public:
        explicit Timer(bool startTiming = true) {
            if (startTiming) {
                this->start();
            }
        }

        void start() {
            this->startTime = clockType::now();
        }

        void reset() {
            this->start();
        }

        double measure() {
            // chrono::duration<double>elapsed = now - start; return elapsed.count();
            return std::chrono::duration_cast<TimeUnit>(clockType::now() - this->startTime).count();
        }

    private:
        typedef std::chrono::high_resolution_clock clockType;
        typedef std::chrono::time_point<clockType> timePoint;

        timePoint startTime;
    };

    typedef Timer<std::chrono::seconds> TimerS;
    typedef Timer<std::chrono::milliseconds> TimerMs;
    typedef Timer<std::chrono::microseconds> TimerUs;
    typedef Timer<std::chrono::nanoseconds> TimerNs;
}

#endif //ANDREIUTILS_TIMER_HPP
