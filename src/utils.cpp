//
// Created by Andrei on 11.11.20.
//

#include <AndreiUtils/utils.h>
#include <iostream>
#include <sstream>

using namespace std;

bool AndreiUtils::stringIsInteger(const string &s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

    char *p;
    strtol(s.c_str(), &p, 10);

    return (*p == 0);
}

int AndreiUtils::stringToInteger(const string &s) {
    if (!stringIsInteger(s)) {
        throw runtime_error("Can not convert string \"" + s + "\" to integer...");
    }
    return stoi(s);
}

bool AndreiUtils::stringIsBool(const string &s) {
    return ((s == "t" || s == "T" || s == "true" || s == "True" || s == "1") ||
            (s == "f" || s == "F" || s == "false" || s == "False" || s == "0"));
}

bool AndreiUtils::stringToBool(const string &s) {
    if (s == "t" || s == "T" || s == "true" || s == "True" || s == "1" || s.empty()) {
        return true;
    } else if (s == "f" || s == "F" || s == "false" || s == "False" || s == "0") {
        return false;
    }
    throw runtime_error("Can not convert string \"" + s + "\" to boolean...");
}

double AndreiUtils::stringToDouble(const string &s) {
    return stod(s);
}

bool AndreiUtils::stringIsFloat(const string &s) {
    std::istringstream iss(s);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

float AndreiUtils::stringToFloat(const string &s) {
    if (!stringIsFloat(s)) {
        throw runtime_error("Can not convert string \"" + s + "\" to float...");
    }
    return stof(s);
}
