//
// Created by Andrei on 13.09.2021.
//

#include <AndreiUtils/classes/Timer.hpp>
#include <stdexcept>

using namespace AndreiUtils;
using namespace std;

Timer::Timer(bool startTiming) {
    if (startTiming) {
        this->start();
    }
}

void Timer::start() {
    this->startTime = clockType::now();
}

void Timer::reset() {
    this->start();
}

double Timer::measure(const string &timeUnit) const {
    if (timeUnit == "d") {
        return this->measure(TimerTimeUnit::DAY);
    } else if (timeUnit == "h") {
        return this->measure(TimerTimeUnit::HOUR);
    } else if (timeUnit == "m") {
        return this->measure(TimerTimeUnit::MINUTE);
    } else if (timeUnit == "s") {
        return this->measure(TimerTimeUnit::SECOND);
    } else if (timeUnit == "ms") {
        return this->measure(TimerTimeUnit::MILLISECOND);
    } else if (timeUnit == "us") {
        return this->measure(TimerTimeUnit::MICROSECOND);
    } else if (timeUnit == "ns") {
        return this->measure(TimerTimeUnit::NANOSECOND);
    } else {
        throw runtime_error("Unknown timeUnit " + timeUnit + "!");
    }
}

double Timer::measure(Timer::TimerTimeUnit timeUnit) const {
    return chrono::duration<double>(clockType::now() - this->startTime).count() *
           Timer::getMultiplicationFactor(timeUnit);
}

double Timer::getMultiplicationFactor(Timer::TimerTimeUnit timeUnit) {
    switch (timeUnit) {
        case DAY: {
            return 1.0 / (24 * 3600);
        }
        case HOUR: {
            return 1.0 / 3600;
        }
        case MINUTE: {
            return 1.0 / 60;
        }
        case SECOND: {
            return 1.0;
        }
        case MILLISECOND: {
            return 1e3;
        }
        case MICROSECOND: {
            return 1e6;
        }
        case NANOSECOND: {
            return 1e9;
        }
        default : {
            throw runtime_error("Unknown TimerTimeUnit value " + to_string(timeUnit));
        }
    }
}
