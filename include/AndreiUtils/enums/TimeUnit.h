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

#ifndef ANDREIUTILS_TIMEUNITS_H
#define ANDREIUTILS_TIMEUNITS_H

#include <string>

namespace AndreiUtils {
    enum TimeUnit {
        DAY,
        HOUR,
        MINUTE,
        SECOND,
        MILLISECOND,
        MICROSECOND,
        NANOSECOND,
    };

    [[nodiscard]] TimeUnit getLargestTimeUnit();

    [[nodiscard]] TimeUnit getSmallestTimeUnit();

    [[nodiscard]] double getMultiplicationFactorRelativeToSeconds(TimeUnit timeUnit);

    [[nodiscard]] std::string convertTimeUnitToString(TimeUnit const &timeUnit);

    [[nodiscard]] TimeUnit convertStringToTimeUnit(std::string const &timeUnit);
} // namespace AndreiUtils

#endif // ANDREIUTILS_TIMEUNITS_H
