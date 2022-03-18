//
// Created by andrei on 08.03.22.
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
        default : {
            throw runtime_error("Unknown InvalidValuesHandlingMode " + to_string(mode));
        }
    }
}

InvalidValuesHandlingMode AndreiUtils::convertStringToInvalidValuesHandlingMode(const string &mode) {
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
