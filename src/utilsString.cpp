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

void AndreiUtils::myError(const string &message) {
    auto v = splitString(message, "\n");
    for (auto &line: v) {
        cerr << "Error: " + line << endl;
    }
    exit(-1);
}

void AndreiUtils::myWarning(const string &message) {
    auto v = splitString(message, "\n");
    for (auto &line: v) {
        cerr << "Warning: " + line << endl;
    }
}

void AndreiUtils::myAssert(bool cond, const string &message) {
    if (!cond) {
        cout << "Assertion failed with message: " + message << endl;
        assert(false);
    }
}

std::string AndreiUtils::operator*(const std::string &s, long long i) {
    string res;
    for (int j = 0; j < i; j++) {
        res += s;
    }
    return res;
}

vector<string> AndreiUtils::splitString(const string &message, const string &splitter) {
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

string AndreiUtils::firstParts(const string &message, const string &partSplitter, int partCount) {
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

string AndreiUtils::lastParts(const string &message, const string &partSplitter, int partCount) {
    vector<string> split = splitString(message, partSplitter);
    int n = split.size(), start = fastMax(0, n - partCount);
    string res;
    for (int i = start; i < n; i++) {
        if (i > start) {
            res += partSplitter;
        }
        res += split[i];
    }
    return res;
}

string AndreiUtils::withoutFirstParts(const string &message, const string &partSplitter, int partCount) {
    vector<string> split = splitString(message, partSplitter);
    string res;
    for (int i = partCount; i < split.size(); i++) {
        if (i > partCount) {
            res += partSplitter;
        }
        res += split[i];
    }
    return res;
}

string AndreiUtils::withoutLastParts(const string &message, const string &partSplitter, int partCount) {
    vector<string> split = splitString(message, partSplitter);
    string res;
    int n = (int) split.size() - partCount;
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            res += partSplitter;
        }
        res += split[i];
    }
    return res;
}

string AndreiUtils::toLowerString(const string &s) {
    string res = s;
    transform(res.begin(), res.end(), res.begin(), [](unsigned char c) { return tolower(c); });
    return res;
}

string AndreiUtils::toUpperString(const string &s) {
    string res = s;
    transform(res.begin(), res.end(), res.begin(), [](unsigned char c) { return toupper(c); });
    return res;
}

bool AndreiUtils::contains(const string &who, const string &what) {
    return (who.find(what) != string::npos);
}

bool AndreiUtils::startsWith(const string &str, const string &startQuery, size_t strSkipFirstCharactersAmount) {
    return (str.rfind(startQuery, strSkipFirstCharactersAmount) == strSkipFirstCharactersAmount);
}

bool AndreiUtils::endsWith(const string &str, const string &endQuery, size_t strIgnoreLastCharactersAmount) {
    if (str.length() >= endQuery.length()) {
        return (0 == str.compare(str.length() - endQuery.length() - strIgnoreLastCharactersAmount, endQuery.length(),
                                 endQuery));
    }
    return false;
}

string AndreiUtils::trim(const string &str, const string &whitespace) {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos) {
        return "";
    } // no content

    const auto strEnd = str.find_last_not_of(whitespace);

    return str.substr(strBegin, strEnd - strBegin + 1);
}

string AndreiUtils::reduce(const string &str, const string &fill, const string &whitespace) {
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

string AndreiUtils::removeComments(const string &s, const string &comment) {
    size_t find_comment = s.find_first_of(comment);
    return s.substr(0, find_comment);
}

string AndreiUtils::replace(const string &s, const string &oldString, const string &newString) {
    size_t startIndex = 0;
    string result = s;
    while ((startIndex = result.find(oldString, startIndex)) != std::string::npos) {
        result.replace(startIndex, oldString.length(), newString);
        startIndex += newString.length();  // Handles case where 'newString' is a substring of 'oldString'
    }
    return result;
}

string AndreiUtils::replaceFirst(const string &s, const string &oldString, const string &newString) {
    string result = s;
    size_t startIndex = result.find(oldString, 0);
    if (startIndex != std::string::npos) {
        result.replace(startIndex, oldString.length(), newString);
    }
    return result;
}

size_t AndreiUtils::stringCount(const string &referenceString, const string &subString) {
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

string AndreiUtils::padLeftUntil(const string &s, const string &pad, int stringSizeLimit, long padTimes) {
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
            nrRepeats = AndreiUtils::fastMin(nrRepeats, padTimes);
        }
    }
    return (pad * nrRepeats) + s;
}

string AndreiUtils::padRightUntil(const string &s, const string &pad, int stringSizeLimit, long padTimes) {
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
            nrRepeats = AndreiUtils::fastMin(nrRepeats, padTimes);
        }
    }
    return s + (pad * nrRepeats);
}
