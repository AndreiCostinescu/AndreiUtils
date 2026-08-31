/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei on 11.11.20.
//

#include <AndreiUtils/utils.h>
#include <iostream>
#include <sstream>

using namespace std;

bool AndreiUtils::stringIsInteger(string const &s) {
    int tmp;
    return stringIsInteger(s, tmp);
}

bool AndreiUtils::stringIsInteger(string const &s, int &res) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        return false;

    char *p;
    long tmpRes = strtol(s.c_str(), &p, 10);

    if (*p == 0) {
        res = (int) tmpRes;
        return true;
    }
    return false;
}

int AndreiUtils::stringToInteger(string const &s) {
    if (!stringIsInteger(s)) {
        throw runtime_error("Can not convert string \"" + s + "\" to integer...");
    }
    return stoi(s);
}

bool AndreiUtils::stringIsBool(string const &s) {
    return ((s == "t" || s == "T" || s == "true" || s == "True" || s == "1") ||
            (s == "f" || s == "F" || s == "false" || s == "False" || s == "0"));
}

bool AndreiUtils::stringIsBool(string const &s, bool &res) {
    if (stringIsBool(s)) {
        res = (s == "t" || s == "T" || s == "true" || s == "True" || s == "1" || s.empty());
        return true;
    }
    return false;
}

bool AndreiUtils::stringToBool(string const &s) {
    if (s == "t" || s == "T" || s == "true" || s == "True" || s == "1" || s.empty()) {
        return true;
    } else if (s == "f" || s == "F" || s == "false" || s == "False" || s == "0") {
        return false;
    }
    throw runtime_error("Can not convert string \"" + s + "\" to boolean...");
}

bool AndreiUtils::stringIsDouble(string const &s) {
    double tmp;
    return stringIsDouble(s, tmp);
}

bool AndreiUtils::stringIsDouble(string const &s, double &res) {
    try {
        res = stod(s);
        return true;
    } catch (exception &e) {
    }
    return false;
}

double AndreiUtils::stringToDouble(string const &s) { return stod(s); }

bool AndreiUtils::stringIsFloat(string const &s) {
    std::istringstream iss(s);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

bool AndreiUtils::stringIsFloat(string const &s, float &res) {
    if (stringIsFloat(s)) {
        res = stof(s);
    }
    return false;
}

float AndreiUtils::stringToFloat(string const &s) {
    if (!stringIsFloat(s)) {
        throw runtime_error("Can not convert string \"" + s + "\" to float...");
    }
    return stof(s);
}
