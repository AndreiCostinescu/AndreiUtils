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
// Created by Andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <vector>

namespace AndreiUtils {
    class TrajectoryBase {
    public:
        explicit TrajectoryBase(int size = 0);

        explicit TrajectoryBase(std::vector<double> const &times);

        explicit TrajectoryBase(std::vector<double> &&times);

        explicit TrajectoryBase(std::vector<double> *times);

        TrajectoryBase(TrajectoryBase const &other);

        TrajectoryBase(TrajectoryBase &&other) noexcept;

        virtual ~TrajectoryBase();

        TrajectoryBase &operator=(TrajectoryBase const &other);

        TrajectoryBase &operator=(TrajectoryBase &&other) noexcept;

        [[nodiscard]] virtual std::shared_ptr<TrajectoryBase> clone() const;

        // just reserves data in the arrays; does NOT change the trajectory size (e.g. with empty elements)!
        virtual void reserveNewSize(size_t newSize);

        [[nodiscard]] std::vector<double> const &getTimes() const;

        [[nodiscard]] std::vector<double> &getTimes();

        [[nodiscard]] size_t const &getSize() const;

    protected:
        void addNewData(std::vector<double> const &newTimes);

        void addNewData(std::vector<double> &&newTimes);

        void addNewDatum(double const &newTime);

        void addNewDatum(double &&newTime);

        void updatePointers(TrajectoryBase const &other);

        void updatePointers(TrajectoryBase &&other);

        std::shared_ptr<std::vector<double>> timesData;

        std::vector<double> *times;
        size_t size; // actual size of the trajectory; may be less than poses.size()
    };
} // namespace AndreiUtils
