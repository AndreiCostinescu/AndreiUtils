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
// Created by Andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/trajectory/TrajectoryBase.h>
#include <vector>

namespace AndreiUtils {
    class CartesianTrajectory : public TrajectoryBase {
    public:
        explicit CartesianTrajectory(int size = 0);

        CartesianTrajectory(std::vector<Posed> const &poses, std::vector<double> const &times);

        CartesianTrajectory(std::vector<Posed> &&poses, std::vector<double> &&times);

        CartesianTrajectory(std::vector<Posed> *poses, std::vector<double> *times);

        CartesianTrajectory(CartesianTrajectory const &other);

        CartesianTrajectory(CartesianTrajectory &&other) noexcept;

        ~CartesianTrajectory() override;

        CartesianTrajectory &operator=(CartesianTrajectory const &other);

        CartesianTrajectory &operator=(CartesianTrajectory &&other) noexcept;

        [[nodiscard]] std::shared_ptr<TrajectoryBase> clone() const override;

        // just reserves data in the arrays; does NOT change the trajectory size (e.g. with empty elements)!
        void reserveNewSize(size_t newSize) override;

        void addNewData(std::vector<Posed> const &newPoses, std::vector<double> const &newTimes);

        void addNewData(std::vector<Posed> &&newPoses, std::vector<double> &&newTimes);

        void addNewDatum(Posed const &newPose, double const &newTime);

        void addNewDatum(Posed &&newPose, double &&newTime);

        [[nodiscard]] std::vector<Posed> const &getPoses() const;

        [[nodiscard]] std::vector<Posed> &getPoses();

    protected:
        void updatePointers(CartesianTrajectory const &other);

        void updatePointers(CartesianTrajectory &&other);

        std::shared_ptr<std::vector<Posed>> posesData;
        std::vector<Posed> *poses;
    };
} // namespace AndreiUtils
