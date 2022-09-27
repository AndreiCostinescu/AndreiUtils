//
// Created by Andrei on 18.03.22.
//

#ifndef ANDREIUTILS_INVALIDVALUESHANDLINGMODE_H
#define ANDREIUTILS_INVALIDVALUESHANDLINGMODE_H

#include <string>

namespace AndreiUtils {
    enum InvalidValuesHandlingMode {
        IGNORE_INVALID,
        SKIP_INVALID,
        FAIL_UPON_INVALID,
    };

    std::string convertInvalidValuesHandlingModeToString(const InvalidValuesHandlingMode &mode);

    InvalidValuesHandlingMode convertStringToInvalidValuesHandlingMode(const std::string &mode);
}

#endif //ANDREIUTILS_INVALIDVALUESHANDLINGMODE_H
