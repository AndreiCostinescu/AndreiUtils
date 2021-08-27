//
// Created by andrei on 11.11.20.
//

#ifndef ANDREIUTILS_UTILS_H
#define ANDREIUTILS_UTILS_H

#include <atomic>
#include <ctime>
#include <string>

extern std::string configDirectoryLocation;

bool stringIsInteger(const std::string &s);

int stringToInteger(const std::string &s);

bool stringToBool(const std::string &s);

double stringToDouble(const std::string &s);

bool stringIsFloat(const std::string &s);

float stringToFloat(const std::string &s);

#endif //ANDREIUTILS_UTILS_H
