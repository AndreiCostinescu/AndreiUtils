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
// Created by Andrei on 13.10.22.
//

#ifndef ANDREIUTILS_SYMMETRY_H
#define ANDREIUTILS_SYMMETRY_H

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <string>
#include <tuple>

namespace AndreiUtils {
    class Symmetry {
    public:
        Symmetry();

        [[nodiscard]] bool operator==(Symmetry const &other) const;

        Posed getSymmetricTransformation(double rangeValue) const;

        Posed getSymmetricTransformation() const;

        std::vector<Posed> createSymmetricPoses(std::vector<Posed> const &poses, double const &rangeValue) const;

        std::vector<Posed> createSymmetricPoses(std::vector<Posed> const &poses) const;

        Posed createSymmetricPose(Posed const &pose, double const &rangeValue) const;

        Posed createSymmetricPose(Posed const &pose) const;

        bool checkIfOrientationFitsSymmetry(Eigen::Quaterniond const &q, double axisSimilarityThreshold) const;

        std::string type;
        Eigen::Vector3d axis, axisDisplacementFromOrigin;
        std::pair<double, double> range;

    protected:
        static double const pi_2;
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_SYMMETRY_H
