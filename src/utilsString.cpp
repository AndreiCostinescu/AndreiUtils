//
// Created by Andrei on 26.08.21.
//

#include <AndreiUtils/utilsString.h>
#include <algorithm>
#include <AndreiUtils/utils.hpp>
#include <cassert>
#include <iostream>

using namespace std;

string AndreiUtils::tab = "    ";

void AndreiUtils::myError(string const &message) {
    auto v = splitString(message, "\n");
    for (auto &line: v) {
        cerr << "Error: " + line << endl;
    }
    exit(-1);
}

void AndreiUtils::myWarning(string const &message) {
    auto v = splitString(message, "\n");
    for (auto &line: v) {
        cerr << "Warning: " + line << endl;
    }
}

void AndreiUtils::myAssert(bool cond, string const &message) {
    if (!cond) {
        cout << "Assertion failed with message: " + message << endl;
        assert(false);
    }
}

std::string AndreiUtils::operator*(string const &s, long long i) {
    string res;
    for (int j = 0; j < i; j++) {
        res += s;
    }
    return res;
}

std::string AndreiUtils::repeat(char const *s, long long i) {
    return string(s) * i;
}

vector<string> AndreiUtils::splitString(string const &message, string const &splitter) {
    // cout << "Message length: " << message.size() << endl;
    vector<string> res;
    size_t start = 0;
    size_t end = message.find(splitter);
    while (start < message.size() && start != string::npos) {
        auto split = message.substr(start, end - start);
        // cout << "\"" << split << "\"; start = " << start << "; end = " << end << endl;
        res.push_back(split);
        start = end + ((string::npos - end >= splitter.size()) ? splitter.size() : 0);
        end = message.find(splitter, start);
    }
    return res;
}

std::string AndreiUtils::joinStrings(vector<string> const &strings, string const &joiner) {
    if (strings.empty()) return "";
    string res = strings[0];
    for (int i = 1; i < strings.size(); ++i) {
        res += joiner + res[i];
    }
    return res;
}

string AndreiUtils::firstParts(string const &message, string const &partSplitter, int partCount) {
    vector<string> split = splitString(message, partSplitter);
    string res;
    for (int i = 0; i < fastMin(split.size(), (size_t) partCount); i++) {
        if (i > 0) {
            res += partSplitter;
        }
        res += split[i];
    }
    return res;
}

string AndreiUtils::lastParts(string const &message, string const &partSplitter, int partCount) {
    vector<string> split = splitString(message, partSplitter);
    int n = (int) split.size(), start = fastMax(0, n - partCount);
    string res;
    for (int i = start; i < n; i++) {
        if (i > start) {
            res += partSplitter;
        }
        res += split[i];
    }
    return res;
}

string AndreiUtils::withoutFirstParts(string const &message, string const &partSplitter, int partsToRemoveFromStart) {
    if (partsToRemoveFromStart < 0) {
        return message;
    }
    vector<string> split = splitString(message, partSplitter);
    string res;
    for (int i = partsToRemoveFromStart; i < split.size(); i++) {
        if (i > partsToRemoveFromStart) {
            res += partSplitter;
        }
        res += split[i];
    }
    return res;
}

string AndreiUtils::withoutLastParts(string const &message, string const &partSplitter, int partsToRemoveFromEnd) {
    if (partsToRemoveFromEnd < 0) {
        return message;
    }
    vector<string> split = splitString(message, partSplitter);
    string res;
    int n = (int) split.size() - AndreiUtils::fastMax(partsToRemoveFromEnd, 0);
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            res += partSplitter;
        }
        res += split[i];
    }
    return res;
}

string AndreiUtils::removeFromStart(string const &message, string const &startPart) {
    size_t const &startPartSize = startPart.size();
    if (message.size() >= startPartSize && message.substr(0, startPartSize) == startPart) {
        return message.substr(startPartSize, std::string::npos);
    }
    return message;
}

string AndreiUtils::removeFromEnd(string const &message, string const &endPart) {
    size_t stringSize = message.size(), endPartSize = endPart.size();
    if (stringSize >= endPartSize && message.substr(stringSize - endPartSize, endPartSize) == endPart) {
        return message.substr(0, stringSize - endPartSize);
    }
    return message;
}

string AndreiUtils::toLowerString(string const &s) {
    string res = s;
    transform(res.begin(), res.end(), res.begin(), [](unsigned char c) { return tolower(c); });
    return res;
}

string AndreiUtils::toUpperString(string const &s) {
    string res = s;
    transform(res.begin(), res.end(), res.begin(), [](unsigned char c) { return toupper(c); });
    return res;
}

bool AndreiUtils::contains(string const &who, string const &what) {
    return (who.find(what) != string::npos);
}

bool AndreiUtils::startsWith(string const &str, string const &startQuery, size_t strSkipFirstCharactersAmount) {
    return (str.rfind(startQuery, strSkipFirstCharactersAmount) == strSkipFirstCharactersAmount);
}

bool AndreiUtils::endsWith(string const &str, string const &endQuery, size_t strIgnoreLastCharactersAmount) {
    if (str.length() >= endQuery.length()) {
        return (0 == str.compare(str.length() - endQuery.length() - strIgnoreLastCharactersAmount, endQuery.length(),
                                 endQuery));
    }
    return false;
}

bool AndreiUtils::endsWithInteger(std::string const &str, int &integer, std::string &restOfString) {
    if (str.back() < '0' || str.back() > '9') {
        return false;
    }
    integer = 0;
    int multiplier = 1;
    for (int index = int(str.size()); index-- > 0; multiplier *= 10) {
        char const &c = str[index];
        if (c < '0' || c > '9') {
            restOfString = str.substr(0, index + 1);
            break;
        }
        integer += multiplier * (c - '0');
    }
    return true;
}

string AndreiUtils::trim(string const &str, string const &whitespace) {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos) {
        return "";
    } // no content

    const auto strEnd = str.find_last_not_of(whitespace);

    return str.substr(strBegin, strEnd - strBegin + 1);
}

string AndreiUtils::reduce(string const &str, string const &fill, string const &whitespace) {
    string res = trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = res.find_first_of(whitespace);
    while (beginSpace != string::npos) {
        const auto endSpace = res.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        res.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = res.find_first_of(whitespace, newStart);
    }

    return res;
}

string AndreiUtils::removeComments(string const &s, string const &comment) {
    size_t find_comment = s.find_first_of(comment);
    return s.substr(0, find_comment);
}

string AndreiUtils::replace(string const &s, string const &oldString, string const &newString) {
    size_t startIndex = 0;
    string result = s;
    while ((startIndex = result.find(oldString, startIndex)) != std::string::npos) {
        result.replace(startIndex, oldString.length(), newString);
        startIndex += newString.length();  // Handles case where 'newString' is a substring of 'oldString'
    }
    return result;
}

string AndreiUtils::replaceFirst(string const &s, string const &oldString, string const &newString) {
    string result = s;
    size_t startIndex = result.find(oldString, 0);
    if (startIndex != std::string::npos) {
        result.replace(startIndex, oldString.length(), newString);
    }
    return result;
}

size_t AndreiUtils::stringCount(string const &referenceString, string const &subString) {
    const size_t step = subString.size();

    size_t count(0);
    size_t pos(0);

    while ((pos = referenceString.find(subString, pos)) != std::string::npos) {
        pos += step;
        ++count;
    }

    return count;
}

pair<string, string> AndreiUtils::getKeyAndValue(string &line) {
    size_t equal_pos = line.find('=');
    string key = trim(line.substr(0, equal_pos));
    string val = trim(line.substr(equal_pos + 1, string::npos));
    return {key, val};
}

string AndreiUtils::padLeftUntil(string const &s, string const &pad, int stringSizeLimit, long padTimes) {
    long nrRepeats;
    if (stringSizeLimit == -1) {
        if (padTimes < 0) {
            nrRepeats = 1;
        } else {
            nrRepeats = padTimes;
        }
    } else {
        nrRepeats = (stringSizeLimit - (long) s.size()) / (long) pad.size();
        if (padTimes >= 0) {
            nrRepeats = fastMin(nrRepeats, padTimes);
        }
    }
    return (pad * nrRepeats) + s;
}

string AndreiUtils::padRightUntil(string const &s, string const &pad, int stringSizeLimit, long padTimes) {
    long nrRepeats;
    if (stringSizeLimit == -1) {
        if (padTimes < 0) {
            nrRepeats = 1;
        } else {
            nrRepeats = padTimes;
        }
    } else {
        nrRepeats = (stringSizeLimit - (long) s.size()) / (long) pad.size();
        if (padTimes >= 0) {
            nrRepeats = fastMin(nrRepeats, padTimes);
        }
    }
    return s + (pad * nrRepeats);
}

string AndreiUtils::removeLeft(string const &s, long count) {
    if (count <= 0) {
        return s;
    }
    if (s.size() <= count) {
        return "";
    }
    return {s.begin() + count, s.end()};
}

string AndreiUtils::removeRight(string const &s, long count) {
    if (count <= 0) {
        return s;
    }
    if (s.size() <= count) {
        return "";
    }
    return {s.begin(), s.end() - count};
}

