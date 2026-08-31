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
// Created by Andrei on 16.06.23.
//

#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace AndreiUtils {
    class IndexIntervalSeries {
    public:
        IndexIntervalSeries();

        explicit IndexIntervalSeries(int maxNumberOfConsecutiveMissingFramesForSameInterval);

        virtual void addIndex(int newIndex);

        virtual void removeTooShortIntervals(int minIntervalSize);

        [[nodiscard]] std::string toString(int getOnlyLatest = -1) const;

        [[nodiscard]] size_t getSize() const;

        [[nodiscard]] std::vector<std::pair<int, int>> &getSeries();

        [[nodiscard]] std::vector<std::pair<int, int>> const &getSeries() const;

    protected:
        int maxNumberOfConsecutiveMissingFramesForSameInterval;
        std::vector<std::pair<int, int>> series;
    };
} // namespace AndreiUtils
