//
// Created by andrei on 25.01.22.
//

#include <AndreiUtils/classes/TimePoint.hpp>
#include <cstdint>
#include <iostream>
#include <stdexcept>

using namespace AndreiUtils;
using namespace std;
using namespace std::chrono;

typedef chrono::duration<int, ratio<3600 * 24>> days;

TimePoint::TimePoint() = default;

TimePoint::TimePoint(const SystemTimePoint &timePoint) : time_point(timePoint) {}

TimePoint &TimePoint::operator=(const SystemTimePoint &other) {
    if (&other == this) {
        return *this;
    }
    ((SystemTimePoint) (*this)) = other;
    return *this;
}

TimePoint TimePoint::addDelta(double time, const std::string &timeUnit) {
    return this->addDelta(time, convertStringToTimeUnit(timeUnit));
}

TimePoint TimePoint::addDelta(double time, TimeUnit unit) {
    switch (unit) {
        case DAY: {
            return TimePoint((*this) + duration_cast<days>(chrono::duration<double>(time)));
        }
        case HOUR: {
            return TimePoint((*this) + duration_cast<chrono::hours>(chrono::duration<double>(time)));
        }
        case MINUTE: {
            return TimePoint((*this) + duration_cast<chrono::minutes>(chrono::duration<double>(time)));
        }
        case SECOND: {
            return TimePoint((*this) + chrono::duration_cast<chrono::seconds>(chrono::duration<double>(time)));
        }
        case MILLISECOND: {
            chrono::duration_cast<chrono::milliseconds>(this->time_since_epoch());
            
            return TimePoint((*this) + chrono::duration_cast<chrono::milliseconds>(chrono::duration<double>(time)));
        }
        case MICROSECOND: {
            return TimePoint((*this) + chrono::duration_cast<chrono::microseconds>(chrono::duration<double>(time)));
        }
        case NANOSECOND: {
            return TimePoint((*this) + chrono::duration_cast<chrono::nanoseconds>(chrono::duration<double>(time)));
        }
        default : {
            throw runtime_error("Unknown TimeUnit " + to_string(unit));
        }
    }
}

SystemTimePoint TimePoint::getSystemTimePoint() const {
    return *this;
}
