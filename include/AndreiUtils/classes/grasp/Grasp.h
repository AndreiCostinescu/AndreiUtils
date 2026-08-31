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
// Created by Andrei on 11.10.22.
//

#pragma once

#include <AndreiUtils/classes/DualQuaternion.hpp>

namespace nlohmann {
    template<typename T, typename SFINAE>
    struct adl_serializer;
}

namespace AndreiUtils {
    class Grasp {
    public:
        friend nlohmann::adl_serializer<Grasp, void>;

        explicit Grasp(std::vector<AndreiUtils::Posed> const &graspPointPoses = {}, bool hasGraspAngleRange = false,
                       std::pair<double, double> const &graspAngleRange = {0, 0});

        virtual ~Grasp();

        [[nodiscard]] bool operator==(Grasp const &other) const;

        [[nodiscard]] size_t getNrPoints() const;

        virtual void update();

        [[nodiscard]] std::vector<AndreiUtils::Posed> const &getCurrentGraspPointPoses();

        [[nodiscard]] std::vector<AndreiUtils::Posed> const &getGraspPointPoses() const;

        void setRelativeTo(AndreiUtils::Posed graspRelativeTo);

        void composeRelativeTo(AndreiUtils::Posed const &graspRelativeTo);

        [[nodiscard]] std::pair<double, double> const &getGraspAngleRange() const;

        [[nodiscard]] std::pair<double, double> &getGraspAngleRange();

        [[nodiscard]] bool hasAngleRange() const;

    protected:
        void updateGraspPointPosesFromRelativeTo();

        AndreiUtils::Posed relativeTo;
        std::vector<AndreiUtils::Posed> graspPointPoses, originalGraspPointPoses;
        std::pair<double, double> graspAngleRange;
        bool hasGraspAngleRange;
    };
} // namespace AndreiUtils
