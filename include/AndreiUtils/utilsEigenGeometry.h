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

    Eigen::Matrix<double, 4, 3> quaternionDerivativeWithRespectToEulerAnglesRotationOrderXYZ(
            Eigen::Vector3d const &euler);

    Eigen::Matrix<double, 4, 3> quaternionDerivativeWithRespectToEulerAnglesRotationOrderZYX(
            Eigen::Vector3d const &euler, bool componentOrderXYZ);

    Eigen::Vector3d getAngularVelocity(Eigen::Quaterniond const &q1, Eigen::Quaterniond const &q2, double deltaT);

    Eigen::Vector3d getAngularVelocity(Eigen::Quaterniond const &deltaQ, double deltaT);

    Eigen::Quaterniond quaternionFromAngularVelocity(Eigen::Vector3d const &w, double deltaT);

    bool inContact(Eigen::Vector3d const &p1, Eigen::Vector3d const &p2, double threshold);

    bool inContact(Eigen::Vector3d const &p1, float const (&p2)[3], double threshold);

    bool inContact(Eigen::Vector3d const &p1, Eigen::Vector3d const &p2, double threshold, double &distance);

    bool inContact(Eigen::Vector3d const &p1, float const (&p2)[3], double threshold, double &distance);
}
