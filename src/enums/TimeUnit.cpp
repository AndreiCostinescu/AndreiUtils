// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 25.01.22.
//

#include <AndreiUtils/enums/TimeUnit.h>
#include <stdexcept>
#include <string>

using namespace AndreiUtils;
using namespace std;

AndreiUtils::TimeUnit AndreiUtils::getLargestTimeUnit() { return AndreiUtils::TimeUnit::DAY; }

AndreiUtils::TimeUnit AndreiUtils::getSmallestTimeUnit() { return AndreiUtils::TimeUnit::NANOSECOND; }

double AndreiUtils::getMultiplicationFactorRelativeToSeconds(TimeUnit timeUnit) {
    switch (timeUnit) {
        case DAY: {
            return 1.0 / (24 * 3600);
        }
        case HOUR: {
            return 1.0 / 3600;
        }
        case MINUTE: {
            return 1.0 / 60;
        }
        case SECOND: {
            return 1.0;
        }
        case MILLISECOND: {
            return 1e3;
        }
        case MICROSECOND: {
            return 1e6;
        }
        case NANOSECOND: {
            return 1e9;
        }
        default: {
            throw runtime_error("Unknown TimeUnit value " + to_string(timeUnit));
        }
    }
}

std::string AndreiUtils::convertTimeUnitToString(TimeUnit const &timeUnit) {
    switch (timeUnit) {
        case DAY: {
            return "d";
        }
        case HOUR: {
            return "h";
        }
        case MINUTE: {
            return "min";
        }
        case SECOND: {
            return "s";
        }
        case MILLISECOND: {
            return "ms";
        }
        case MICROSECOND: {
            return "us";
        }
        case NANOSECOND: {
            return "ns";
        }
        default: {
            throw runtime_error("Unknown TimeUnit value " + to_string(timeUnit));
        }
    }
}

TimeUnit AndreiUtils::convertStringToTimeUnit(std::string const &timeUnit) {
    if (timeUnit == "d") {
        return TimeUnit::DAY;
    } else if (timeUnit == "h") {
        return TimeUnit::HOUR;
    } else if (timeUnit == "min") {
        return TimeUnit::MINUTE;
    } else if (timeUnit == "s") {
        return TimeUnit::SECOND;
    } else if (timeUnit == "ms") {
        return TimeUnit::MILLISECOND;
    } else if (timeUnit == "us") {
        return TimeUnit::MICROSECOND;
    } else if (timeUnit == "ns") {
        return TimeUnit::NANOSECOND;
    } else {
        throw runtime_error("Unknown TimeUnit string value " + timeUnit);
    }
}
