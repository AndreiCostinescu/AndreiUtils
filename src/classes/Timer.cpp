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
    return this->measure(convertStringToTimeUnit(timeUnit));
}

double Timer::measure(TimeUnit timeUnit) const {
    return chrono::duration<double>(clockType::now() - this->startTime).count() *
           getMultiplicationFactorRelativeToSeconds(timeUnit);
}
