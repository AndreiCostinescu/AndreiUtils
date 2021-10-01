//
// Created by Andrei on 27.08.21.
//

#include <AndreiUtils/utilsTime.h>
#include <cassert>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;

string AndreiUtils::convertChronoToString(const time_point<system_clock> &time, const string &format) {
    time_t timeStruct;
    stringstream ss;
    timeStruct = system_clock::to_time_t(time);
    ss.clear();
    ss.str(string());
    ss << put_time(localtime(&timeStruct), format.c_str());
    return ss.str();
}

time_point<system_clock> AndreiUtils::convertStringToChrono(const string &time, const string &format) {
    tm tm = {};
    stringstream ss;
    ss.clear();
    ss.str(string());
    ss << time;
    ss >> get_time(&tm, format.c_str());
    return system_clock::from_time_t(mktime(&tm));
}

void AndreiUtils::getDateFromTime(struct tm *&t, time_t time, int &year) {
    t = gmtime(&time);
    year = t->tm_year + 1900;
}

void AndreiUtils::getDateFromTime(struct tm *&t, time_t time, int &year, int &month) {
    AndreiUtils::getDateFromTime(t, time, year);
    month = t->tm_mon + 1;
}

void AndreiUtils::getDateFromTime(struct tm *&t, time_t time, int &year, int &month, int &day) {
    AndreiUtils::getDateFromTime(t, time, year, month);
    day = t->tm_mday;
}

void AndreiUtils::getDateFromTime(struct tm *&t, time_t time, int &year, int &month, int &day, int &hour) {
    AndreiUtils::getDateFromTime(t, time, year, month, day);
    hour = t->tm_hour;
}

void AndreiUtils::getDateFromTime(struct tm *&t, time_t time, int &year, int &month, int &day, int &hour, int &min) {
    AndreiUtils::getDateFromTime(t, time, year, month, day, hour);
    min = t->tm_min;
}

void AndreiUtils::getDateFromTime(struct tm *&t, time_t time, int &year, int &month, int &day, int &hour, int &min,
                                   int &sec) {
    AndreiUtils::getDateFromTime(t, time, year, month, day, hour, min);
    sec = t->tm_sec;
}

void AndreiUtils::getDateFromTime(time_t time, int &year, int &month, int &day, int &hour, int &min, int &sec) {
    struct tm *t = nullptr;
    AndreiUtils::getDateFromTime(t, time, year, month, day, hour, min, sec);
}

void AndreiUtils::getDateFromNow(int &year, int &month, int &day, int &hour, int &min, int &sec) {
    auto now = time(nullptr);
    AndreiUtils::getDateFromTime(now, year, month, day, hour, min, sec);
}

void AndreiUtils::getDateFromTime(time_t time, int &year, int &month, int &day) {
    struct tm *t = nullptr;
    AndreiUtils::getDateFromTime(t, time, year, month, day);
}

void AndreiUtils::getDateFromNow(int &year, int &month, int &day) {
    auto now = time(nullptr);
    AndreiUtils::getDateFromTime(now, year, month, day);
}

void AndreiUtils::updateTime(const clock_t &newTime, clock_t &prevTime, bool updatePrevTime, float *difference) {
    assert(updatePrevTime || difference != nullptr);
    if (difference != nullptr) {
        *difference = ((float) (newTime - prevTime)) / CLOCKS_PER_SEC;
    }
    if (updatePrevTime) {
        prevTime = newTime;
    }
}

void AndreiUtils::updateTime(const clock_t &newTime, clock_t &prevTime, bool updatePrevTime, double *difference) {
    assert(updatePrevTime || difference != nullptr);
    if (difference != nullptr) {
        *difference = ((double) (newTime - prevTime)) / CLOCKS_PER_SEC;
    }
    if (updatePrevTime) {
        prevTime = newTime;
    }
}

clock_t AndreiUtils::createDeltaTime(double fps, double deltaSec) {
    if (fps > 0) {
        return (clock_t) (CLOCKS_PER_SEC * 1 / fps);
    }
    return (clock_t) (CLOCKS_PER_SEC * deltaSec);
}
