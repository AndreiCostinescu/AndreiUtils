//
// Created by Andrei on 26.08.21.
//

#ifndef ANDREIUTILS_UTILSSTRING_H
#define ANDREIUTILS_UTILSSTRING_H

#include <string>
#include <vector>

namespace AndreiUtils {
    extern std::string tab;

    void myError(const std::string &message = "");

    void myWarning(const std::string &message = "");

    void myAssert(bool cond, const std::string &message = "");

    std::string operator *(const std::string &s, long long i);

    std::vector<std::string> splitString(const std::string &message, const std::string &splitter);

    std::string firstParts(const std::string &message, const std::string &partSplitter, int partCount);

    std::string lastParts(const std::string &message, const std::string &partSplitter, int partCount);

    std::string withoutFirstParts(const std::string &message, const std::string &partSplitter, int partCount);

    std::string withoutLastParts(const std::string &message, const std::string &partSplitter, int partCount);

    std::string toLowerString(const std::string &s);

    std::string toUpperString(const std::string &s);

    bool contains(const std::string &who, const std::string &what);

    bool startsWith(const std::string &str, const std::string &startQuery, size_t strSkipFirstCharactersAmount = 0);

    bool endsWith(const std::string &str, const std::string &endQuery, size_t strIgnoreLastCharactersAmount = 0);

    std::string trim(const std::string &str, const std::string &whitespace = " \t");

    std::string reduce(const std::string &str, const std::string &fill = " ", const std::string &whitespace = " \t");

    std::string removeComments(const std::string &s, const std::string &comment = "#");

    std::string replace(const std::string &s, const std::string &oldString, const std::string &newString);

    std::string replaceFirst(const std::string &s, const std::string &oldString, const std::string &newString);

    size_t stringCount(const std::string &referenceString, const std::string &subString);

    std::pair<std::string, std::string> getKeyAndValue(std::string &line);

    std::string padLeftUntil(const std::string &s, const std::string &pad, int stringSizeLimit = -1,
                             long padTimes = -1);

    std::string padRightUntil(const std::string &s, const std::string &pad, int stringSizeLimit = -1,
                              long padTimes = -1);
}

#endif //ANDREIUTILS_UTILSSTRING_H
