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
// Created by Andrei on 13.09.2021.
//

#ifndef ANDREIUTILS_TIMER_HPP
#define ANDREIUTILS_TIMER_HPP

#include <AndreiUtils/enums/TimeUnit.h>
#include <chrono>
#include <string>

namespace AndreiUtils {
    class Timer {
    public:
        explicit Timer(bool startTiming = true);

        void start();

        void reset();

        [[nodiscard]] double measure(std::string const &timeUnit = "s") const;

        [[nodiscard]] double measure(TimeUnit timeUnit) const;

        template<class TimerTimeUnit>
        double measure() {
            return std::chrono::duration_cast<TimerTimeUnit>(clockType::now() - this->startTime).count();
        }

    private:
        typedef std::chrono::high_resolution_clock clockType;
        typedef std::chrono::time_point<clockType> timePoint;

        timePoint startTime;
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_TIMER_HPP
