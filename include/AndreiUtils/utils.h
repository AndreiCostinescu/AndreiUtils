//
// Created by Andrei on 11.11.20.
//

#pragma once

#include <string>

namespace AndreiUtils {
    bool stringIsInteger(std::string const &s);

    bool stringIsInteger(std::string const &s, int &res);

    int stringToInteger(std::string const &s);

    bool stringIsBool(std::string const &s);

    bool stringIsBool(std::string const &s, bool &res);

    bool stringToBool(std::string const &s);

    double stringToDouble(std::string const &s);

    bool stringIsFloat(std::string const &s);

    bool stringIsFloat(std::string const &s, float &res);

    float stringToFloat(std::string const &s);
}
