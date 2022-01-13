//
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSTIME_H
#define ANDREIUTILS_UTILSTIME_H

#include <chrono>
#include <string>

namespace AndreiUtils {
    using SystemTimePoint = std::chrono::time_point<std::chrono::system_clock>;

    std::string convertChronoToString(const SystemTimePoint &time, const std::string &format = "%Y-%m-%d-%H-%M-%S");

    std::chrono::time_point<std::chrono::system_clock> convertStringToChrono(
            const std::string &time, const std::string &format = "%Y-%m-%d-%H-%M-%S");

    std::string convertChronoToStringWithSubseconds(
            const SystemTimePoint &time, const std::string &format = "%Y-%m-%d-%H-%M-%S",
            const std::string &subsecondFormat = "%ns", const std::string &joiner = ":");

    std::chrono::time_point<std::chrono::system_clock> convertStringToChronoWithSubseconds(
            const std::string &time, const std::string &format = "%Y-%m-%d-%H-%M-%S",
            const std::string &subsecondFormat = "%ns", const std::string &joiner = ":");

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

    void updateTime(const clock_t &newTime, clock_t &prevTime, bool updatePrevTime = false,
                    float *difference = nullptr);

    void updateTime(const clock_t &newTime, clock_t &prevTime, bool updatePrevTime = false,
                    double *difference = nullptr);

    clock_t createDeltaTime(double fps = 0.0, double deltaSec = 0.0);
}

#endif //ANDREIUTILS_UTILSTIME_H
