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
// Created by Andrei Costinescu on 17.09.21.
//

#pragma once

#include <Eigen/Dense>

namespace AndreiUtils {
    Eigen::Matrix3d xRotation(double angle);

    Eigen::Matrix3d yRotation(double angle);

    Eigen::Matrix3d zRotation(double angle);

    Eigen::Vector3d sampleDirection();

    Eigen::Quaterniond sampleOrientation();

    Eigen::Matrix4d quaternionConjugateDerivative();

    Eigen::Vector3d quaternionLogarithm(Eigen::Quaterniond const &q);

    Eigen::Matrix<double, 3, 4> quaternionLogarithmDerivative(Eigen::Quaterniond const &q);

    Eigen::Matrix4d quaternionProductDerivativeWRTFirst(Eigen::Quaterniond const &q1, Eigen::Quaterniond const &q2);

    Eigen::Matrix4d quaternionProductDerivativeWRTSecond(Eigen::Quaterniond const &q1, Eigen::Quaterniond const &q2);

    Eigen::Quaterniond quaternionFromEulerAnglesRotationOrderXYZ(Eigen::Vector3d const &euler);

    Eigen::Quaterniond quaternionFromEulerAnglesRotationOrderZYX(Eigen::Vector3d const &euler, bool componentOrderXYZ);

    Eigen::Matrix<double, 4, 3>
    quaternionDerivativeWithRespectToEulerAnglesRotationOrderXYZ(Eigen::Vector3d const &euler);

    Eigen::Matrix<double, 4, 3>
    quaternionDerivativeWithRespectToEulerAnglesRotationOrderZYX(Eigen::Vector3d const &euler, bool componentOrderXYZ);

    Eigen::Vector3d getAngularVelocity(Eigen::Quaterniond const &q1, Eigen::Quaterniond const &q2, double deltaT);

    Eigen::Vector3d getAngularVelocity(Eigen::Quaterniond const &deltaQ, double deltaT);

    Eigen::Quaterniond quaternionFromAngularVelocity(Eigen::Vector3d const &w, double deltaT);

    bool inContact(Eigen::Vector3d const &p1, Eigen::Vector3d const &p2, double threshold);

    bool inContact(Eigen::Vector3d const &p1, float const (&p2)[3], double threshold);

    bool inContact(Eigen::Vector3d const &p1, Eigen::Vector3d const &p2, double threshold, double &distance);

    bool inContact(Eigen::Vector3d const &p1, float const (&p2)[3], double threshold, double &distance);
} // namespace AndreiUtils
