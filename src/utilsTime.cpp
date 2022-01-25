//
// Created by Andrei on 27.08.21.
//

#include <AndreiUtils/utilsTime.h>
#include <AndreiUtils/utilsString.h>
#include <cassert>
#include <iomanip>
#include <sstream>

using namespace AndreiUtils;
using namespace std;
using namespace std::chrono;

typedef ratio<3600 * 24> daysRatio;
typedef ratio<3600> hoursRatio;
typedef ratio<60> minutesRatio;

string AndreiUtils::convertChronoToString(const SystemTimePoint &time, const string &format) {
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

string AndreiUtils::convertChronoToStringWithSubseconds(const SystemTimePoint &time, const string &format,
                                                        const string &subsecondFormat, const string &joiner) {
    if (joiner.empty()) {
        AndreiUtils::myWarning("The subsecond joiner string is empty... "
                               "Reconstructing the original time from the string will not work!");
    } else if (AndreiUtils::contains(format, joiner)) {
        AndreiUtils::myWarning("The subsecond joiner string is contained in the time formatter... "
                               "Reconstructing the original time from the string will not work!");
    }
    string res = convertChronoToString(time, format);
    string subsecondRes = subsecondFormat;
    if (!subsecondRes.empty()) {
        if (!joiner.empty() && AndreiUtils::contains(subsecondFormat, joiner)) {
            AndreiUtils::myWarning("The subsecond joiner string is contained in the subsecond time formatter... "
                                   "Reconstructing the original time from the string will not work!");
        }
        auto d = time.time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(d);
        d -= seconds;  // now we are in the subsecond time interval
        auto fullNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(d).count();
        subsecondRes = AndreiUtils::replace(subsecondRes, "%ns",
                                            AndreiUtils::padLeftUntil(to_string(fullNanoseconds), "0", 9));
        subsecondRes = AndreiUtils::replace(subsecondRes, "%pns",
                                            AndreiUtils::padLeftUntil(to_string(fullNanoseconds % 1000), "0", 3));
        fullNanoseconds /= 1000;
        subsecondRes = AndreiUtils::replace(subsecondRes, "%us",
                                            AndreiUtils::padLeftUntil(to_string(fullNanoseconds), "0", 6));
        subsecondRes = AndreiUtils::replace(subsecondRes, "%pus",
                                            AndreiUtils::padLeftUntil(to_string(fullNanoseconds % 1000), "0", 3));
        fullNanoseconds /= 1000;
        subsecondRes = AndreiUtils::replace(subsecondRes, "%ms",
                                            AndreiUtils::padLeftUntil(to_string(fullNanoseconds), "0", 3));
    }
    return res + joiner + subsecondRes;
}

time_point<system_clock> AndreiUtils::convertStringToChronoWithSubseconds(
        const string &time, const string &format, const string &subsecondFormat, const string &joiner) {
    if (joiner.empty()) {
        string message = "The subsecond joiner part is empty... Can not reconstruct subsecond part."
                         "Will reconstruct only using format = " + format;
        AndreiUtils::myWarning(message);
        return convertStringToChrono(time, format);
    }
    auto timeSplits = AndreiUtils::splitString(time, joiner);
    if (timeSplits.size() > 2) {
        AndreiUtils::myWarning("Joiner string \"" + joiner + "\" is contained multiple times in the time \"" +
                               time + "\". Will only use the first two components and try to reconstruct this way...");
    }
    auto res = convertStringToChrono(timeSplits[0], format);
    string timeString = timeSplits[1];
    size_t subsecondFormatSize = subsecondFormat.size(), timeSize = timeString.size();
    // the -3 is because all subsecond formats are at least 3 characters in length
    for (int formatIndex = 0, timeIndex = 0;
         formatIndex < subsecondFormatSize - 2 && timeIndex < timeSize; formatIndex++, timeIndex++) {
        if (subsecondFormat[formatIndex] == timeString[timeIndex] && subsecondFormat[formatIndex] != '%') {
            continue;
        } else if (subsecondFormat[formatIndex] != '%') {
            break;
        }
        assert (subsecondFormat[formatIndex] == '%');
        // until now all indices [formatIndex, formatIndex + 1, formatIndex + 2] are safe to access
        if (subsecondFormat[formatIndex + 1] == '%') {
            if (timeString[timeIndex] == '%') {
                formatIndex++;
                continue;
            } else {
                break;
            }
        } else if (subsecondFormat[formatIndex + 1] == 'p') {
            if (formatIndex + 3 >= subsecondFormatSize) {
                break;
            }
            if (subsecondFormat[formatIndex + 2] == 'u' && subsecondFormat[formatIndex + 3] == 's' &&
                timeIndex + 2 < timeSize) {
                // part microseconds: expecting at three characters more in timeString string
                res += std::chrono::microseconds(stoi(timeString.substr(timeIndex, 3)));
                timeIndex += 2;  // 3 - 1 because there's the timeIndex++ performed in the for-loop
            } else if (subsecondFormat[formatIndex + 2] == 'n' && subsecondFormat[formatIndex + 3] == 's' &&
                       timeIndex + 2 < timeSize) {
                // part nanoseconds: expecting at three characters more in timeString string
                res += std::chrono::nanoseconds(stoi(timeString.substr(timeIndex, 3)));
                timeIndex += 2;  // 3 - 1 because there's the timeIndex++ performed in the for-loop
            } else {
                AndreiUtils::myWarning("Unknown formatter for subsecond values in " + subsecondFormat);
                break;
            }
            formatIndex += 3;
        } else if (subsecondFormat[formatIndex + 1] == 'm' && subsecondFormat[formatIndex + 2] == 's' &&
                   timeIndex + 2 < timeSize) {
            // milliseconds: expecting at three characters more in timeString string
            res += std::chrono::milliseconds(stoi(timeString.substr(timeIndex, 3)));
            timeIndex += 2;  // 3 - 1 because there's the timeIndex++ performed in the for-loop
            formatIndex += 2;
        } else if (subsecondFormat[formatIndex + 1] == 'u' && subsecondFormat[formatIndex + 2] == 's' &&
                   timeIndex + 5 < timeSize) {
            // microseconds: expecting at six characters more in timeString string
            res += std::chrono::microseconds(stoi(timeString.substr(timeIndex, 6)));
            timeIndex += 5;  // 6 - 1 because there's the timeIndex++ performed in the for-loop
            formatIndex += 2;
        } else if (subsecondFormat[formatIndex + 1] == 'n' && subsecondFormat[formatIndex + 2] == 's' &&
                   timeIndex + 8 < timeSize) {
            // nanoseconds: expecting at nine characters more in timeString string
            res += std::chrono::nanoseconds(stoi(timeString.substr(timeIndex, 9)));
            timeIndex += 8;  // 9 - 1 because there's the timeIndex++ performed in the for-loop
            formatIndex += 2;
        } else {
            AndreiUtils::myWarning("Unknown formatter for subsecond values in " + subsecondFormat);
            break;
        }
    }
    return res;
}

SystemTimePoint AndreiUtils::addDeltaTime(const SystemTimePoint &timePoint, double deltaT, const string &timeUnit) {
    return addDeltaTime(timePoint, deltaT, convertStringToTimeUnit(timeUnit));
}

SystemTimePoint AndreiUtils::addDeltaTime(const SystemTimePoint &timePoint, double deltaT, TimeUnit timeUnit) {
    chrono::duration<double> t{};
    switch (timeUnit) {
        case DAY: {
            t = chrono::duration<double, daysRatio>(deltaT);
            break;
        }
        case HOUR: {
            t = chrono::duration<double, hoursRatio>(deltaT);
            break;
        }
        case MINUTE: {
            t = chrono::duration<double, minutesRatio>(deltaT);
            break;
        }
        case SECOND: {
            t = chrono::duration<double>(deltaT);
            break;
        }
        case MILLISECOND: {
            t = chrono::duration<double, milli>(deltaT);
            break;
        }
        case MICROSECOND: {
            t = chrono::duration<double, micro>(deltaT);
            break;
        }
        case NANOSECOND: {
            t = chrono::duration<double, nano>(deltaT);
            break;
        }
        default : {
            throw runtime_error("Unknown TimeUnit " + to_string(timeUnit));
        }
    }
    return timePoint + chrono::duration_cast<nanoseconds>(t);
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
        return (clock_t)(CLOCKS_PER_SEC * 1 / fps);
    }
    return (clock_t)(CLOCKS_PER_SEC * deltaSec);
}
