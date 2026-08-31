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
// Created by Andrei on 08.03.22.
//

#include <AndreiUtils/enums/InvalidValuesHandlingMode.h>
#include <stdexcept>

using namespace AndreiUtils;
using namespace std;

std::string AndreiUtils::convertInvalidValuesHandlingModeToString(const InvalidValuesHandlingMode &mode) {
    switch (mode) {
        case IGNORE_INVALID: {
            return "ignoreInvalid";
        }
        case SKIP_INVALID: {
            return "skipInvalid";
        }
        case FAIL_UPON_INVALID: {
            return "failUponInvalid";
        }
        default: {
            throw runtime_error("Unknown InvalidValuesHandlingMode " + to_string(mode));
        }
    }
}

InvalidValuesHandlingMode AndreiUtils::convertStringToInvalidValuesHandlingMode(string const &mode) {
    if (mode == "ignoreInvalid") {
        return AndreiUtils::InvalidValuesHandlingMode::IGNORE_INVALID;
    } else if (mode == "skipInvalid") {
        return AndreiUtils::InvalidValuesHandlingMode::SKIP_INVALID;
    } else if (mode == "failUponInvalid") {
        return AndreiUtils::InvalidValuesHandlingMode::FAIL_UPON_INVALID;
    } else {
        throw runtime_error("Unknown InvalidValuesHandlingMode: \"" + mode + "\"");
    }
}
