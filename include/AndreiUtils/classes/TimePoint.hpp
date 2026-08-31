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
// Created by Andrei on 06.12.22.
//

#ifndef ANDREIUTILS_TIMEPOINT_HPP
#define ANDREIUTILS_TIMEPOINT_HPP

#include <chrono>

namespace AndreiUtils {
    using SteadyClock = std::chrono::steady_clock;
    using SteadyTimePoint = std::chrono::time_point<SteadyClock>;

    using SystemClock = std::chrono::system_clock;
    using SystemTimePoint = std::chrono::time_point<SystemClock>;

    using HighResClock = std::chrono::high_resolution_clock;
    using HighResTimePoint = std::chrono::time_point<HighResClock>;
} // namespace AndreiUtils

#endif // ANDREIUTILS_TIMEPOINT_HPP
