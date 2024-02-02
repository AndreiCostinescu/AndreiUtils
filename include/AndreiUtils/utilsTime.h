//
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSTIME_H
#define ANDREIUTILS_UTILSTIME_H

#include <AndreiUtils/classes/TimePoint.hpp>
#include <AndreiUtils/enums/TimeUnit.h>
#include <chrono>
#include <string>

namespace AndreiUtils {
    AndreiUtils::HighResTimePoint nowHighResClock();

    AndreiUtils::SteadyTimePoint nowSteadyClock();

    AndreiUtils::SystemTimePoint nowSystemClock();

    AndreiUtils::SystemTimePoint now();

    std::string convertChronoToString(SystemTimePoint const &time, std::string const &format = "%Y-%m-%d-%H-%M-%S");

    SystemTimePoint convertStringToChrono(std::string const &time, std::string const &format = "%Y-%m-%d-%H-%M-%S");

    std::string convertChronoToStringWithSubsecondsCustomJoin(SystemTimePoint const &time, std::string const &joiner);

    SystemTimePoint convertStringToChronoWithSubsecondsCustomJoin(std::string const &time, std::string const &joiner);

    std::string convertChronoToStringWithSubseconds(
            SystemTimePoint const &time, std::string const &format = "%Y-%m-%d-%H-%M-%S",
            std::string const &subsecondFormat = "%ns", std::string const &joiner = ":");

    SystemTimePoint convertStringToChronoWithSubseconds(
            std::string const &time, std::string const &format = "%Y-%m-%d-%H-%M-%S",
            std::string const &subsecondFormat = "%ns", std::string const &joiner = ":");

    SystemTimePoint addDeltaTime(SystemTimePoint const &timePoint, double deltaT, std::string const &timeUnit);

    SystemTimePoint addDeltaTime(SystemTimePoint const &timePoint, double deltaT, TimeUnit timeUnit);

    SystemTimePoint getTimePoint(double t, std::string const &timeUnit);

    SystemTimePoint getTimePoint(double t, TimeUnit timeUnit);

    double getTime(SystemTimePoint const &t, std::string const &timeUnit);

    double getTime(SystemTimePoint const &t, TimeUnit timeUnit);

    double getTimeDiff(SystemTimePoint const &t1, SystemTimePoint const &t2, std::string const &timeUnit);

    double getTimeDiff(SystemTimePoint const &t1, SystemTimePoint const &t2, TimeUnit timeUnit);

    double getTime(double t, std::string const &timeUnit);

    double getTime(double t, TimeUnit timeUnit);

    void getDateFromTime(struct tm *&t, time_t time, int &year);

    void getDateFromTime(struct tm *&t, time_t time, int &year, int &month);

    void getDateFromTime(struct tm *&t, time_t time, int &year, int &month, int &day);

    void getDateFromTime(struct tm *&t, time_t time, int &year, int &month, int &day, int &hour);

    void getDateFromTime(struct tm *&t, time_t time, int &year, int &month, int &day, int &hour, int &min);

    void getDateFromTime(struct tm *&t, time_t time, int &year, int &month, int &day, int &hour, int &min, int &sec);

    void getDateFromTime(time_t time, int &year, int &month, int &day, int &hour, int &min, int &sec);

    void getDateFromNow(int &year, int &month, int &day, int &hour, int &min, int &sec);

    void getDateFromTime(time_t time, int &year, int &month, int &day);

    void getDateFromNow(int &year, int &month, int &day);

    void updateTime(clock_t const &newTime, clock_t &prevTime, bool updatePrevTime = false,
                    float *difference = nullptr);

    void updateTime(clock_t const &newTime, clock_t &prevTime, bool updatePrevTime = false,
                    double *difference = nullptr);

    clock_t createDeltaTime(double fps = 0.0, double deltaSec = 0.0);
}

#endif //ANDREIUTILS_UTILSTIME_H
