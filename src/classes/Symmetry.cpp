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
// Created by Andrei on 13.10.22.
//

#include <AndreiUtils/classes/RandomNumberGenerator.hpp>
#include <AndreiUtils/classes/Symmetry.h>

using namespace AndreiUtils;

Symmetry::Symmetry() = default;

bool Symmetry::operator==(Symmetry const &other) const {
    return this->type == other.type && this->axis == other.axis &&
           this->axisDisplacementFromOrigin == other.axisDisplacementFromOrigin && this->range == other.range;
}

Posed Symmetry::getSymmetricTransformation(double rangeValue) const {
    rangeValue = AndreiUtils::clamp(rangeValue, this->range.first, this->range.second);
    Posed randomPose;
    if (this->type == "rotation") {
        // TODO: what about the axisDisplacement?!
        randomPose =
                Posed(Eigen::Quaterniond(Eigen::AngleAxis<double>(rangeValue, this->axis)), Eigen::Vector3d::Zero());
    } else {
        // TODO: what about the axisDisplacement?!
        randomPose = Posed(qIdentity<double>(), rangeValue * this->axis);
    }
    return randomPose;
}

Posed Symmetry::getSymmetricTransformation() const {
    double randomRange = RandomNumberGenerator<double>(this->range.first, this->range.second).sample();
    return this->getSymmetricTransformation(randomRange);
}

std::vector<Posed> Symmetry::createSymmetricPoses(std::vector<Posed> const &poses, double const &rangeValue) const {
    std::vector<Posed> newPoses;
    auto randomPose = this->getSymmetricTransformation(rangeValue);
    for (auto const &pose: poses) {
        newPoses.push_back(randomPose * pose);
    }
    return newPoses;
}

std::vector<Posed> Symmetry::createSymmetricPoses(std::vector<Posed> const &poses) const {
    std::vector<Posed> newPoses;
    auto randomPose = this->getSymmetricTransformation();
    for (auto const &pose: poses) {
        newPoses.push_back(randomPose * pose);
    }
    return newPoses;
}

Posed Symmetry::createSymmetricPose(Posed const &pose, double const &rangeValue) const {
    return this->getSymmetricTransformation(rangeValue) * pose;
}

Posed Symmetry::createSymmetricPose(Posed const &pose) const { return this->getSymmetricTransformation() * pose; }

bool Symmetry::checkIfOrientationFitsSymmetry(Eigen::Quaterniond const &q, double axisSimilarityThreshold) const {
    Eigen::AngleAxisd qAxisAngle(q);
    double &qAngle = qAxisAngle.angle(); // <- this is always in [0, pi]
    if (AndreiUtils::equal<double>(qAngle, 0)) {
        return true;
    }
    Eigen::Vector3d &qAxis = qAxisAngle.axis();

    bool posCheck = (qAxis - this->axis).norm() <= axisSimilarityThreshold;
    bool negCheck = (qAxis + this->axis).norm() <= axisSimilarityThreshold;
    if (!posCheck && !negCheck) {
        return false;
    }

    if (posCheck) {
        // symmetry axis == qAxis
        return lessEqual(this->range.first, qAngle) && lessEqual(qAngle, this->range.second);
    }

    // symmetry axis == -qAxis
    return lessEqual(Symmetry::pi_2 - this->range.second, qAngle) &&
           lessEqual(qAngle, Symmetry::pi_2 - this->range.first);
}

double const Symmetry::pi_2(2 * M_PI);
