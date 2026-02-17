//
// Created by Andrei on 26.08.21.
//

#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace AndreiUtils {
    extern std::string tab;

    void myError(std::string const &message = "");

    void myWarning(std::string const &message = "");

    void myAssert(bool cond, std::string const &message = "");

    std::string operator*(std::string const &s, long long i);

    std::string repeat(char const *s, long long i);

    std::vector<std::string> splitString(std::string const &message, std::string const &splitter);

    std::string joinStrings(std::vector<std::string> const &strings, std::string const &joiner);

    std::string firstParts(std::string const &message, std::string const &partSplitter, int partCount);

    std::string lastParts(std::string const &message, std::string const &partSplitter, int partCount);

    std::string withoutFirstParts(std::string const &message, std::string const &partSplitter,
                                  int partsToRemoveFromStart);

    std::string withoutLastParts(std::string const &message, std::string const &partSplitter, int partsToRemoveFromEnd);

    std::string removeFromStart(std::string const &message, std::string const &startPart);

    std::string removeFromEnd(std::string const &message, std::string const &endPart);

    std::string toLowerString(std::string const &s);

    std::string toUpperString(std::string const &s);

    std::string capitalize(std::string const &s);

    std::string makeStartUpperCase(std::string const &s);

    std::string makeStartLowerCase(std::string const &s);

    bool contains(std::string const &who, std::string const &what);

    bool startsWith(std::string const &str, std::string const &startQuery, size_t strSkipFirstCharactersAmount = 0);

    bool endsWith(std::string const &str, std::string const &endQuery, size_t strIgnoreLastCharactersAmount = 0);

    bool endsWithInteger(std::string const &str, int &integer, std::string &restOfString);

    bool endsWithInteger(std::string const &str, long &longVal, std::string &restOfString);

    std::string trim(std::string const &str, std::string const &whitespace = " \t");

    std::string trimEnd(std::string const &str, std::string const &whitespace = " \t");

    std::string trimStart(std::string const &str, std::string const &whitespace = " \t");

    std::string strip(std::string const &str, std::string const &whitespace = " \t");

    std::string stripEnd(std::string const &str, std::string const &whitespace = " \t");

    std::string stripStart(std::string const &str, std::string const &whitespace = " \t");

    std::string reduce(std::string const &str, std::string const &fill = " ", std::string const &whitespace = " \t");

    std::string removeComments(std::string const &s, std::string const &comment = "#");

    std::string replace(std::string const &s, std::string const &oldString, std::string const &newString);

    std::string replaceFirst(std::string const &s, std::string const &oldString, std::string const &newString);

    size_t stringCount(std::string const &referenceString, std::string const &subString);

    std::pair<std::string, std::string> getKeyAndValue(std::string &line, std::string const &keySplitter = "=");

    std::string padLeftUntil(std::string const &s, std::string const &pad, int stringSizeLimit = -1,
                             long padTimes = -1);

    std::string padRightUntil(std::string const &s, std::string const &pad, int stringSizeLimit = -1,
                              long padTimes = -1);

    std::string removeLeft(std::string const &s, long count);

    std::string removeRight(std::string const &s, long count);

    bool isStringBooleanTrueValue(std::string const &s);

    bool isStringBooleanFalseValue(std::string const &s);

    bool isStringAFloatingPointValue(std::string const &str, double &value);

    bool isInputStringStreamAtEnd(std::istringstream &ss);

    std::string surroundWithIfNotAlready(std::string const &toSurround, std::string const &surroundingString);
}
