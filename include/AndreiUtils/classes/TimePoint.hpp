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
}

#endif //ANDREIUTILS_TIMEPOINT_HPP
