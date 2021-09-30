//
// Created by Andrei on 17.09.21.
//

#ifndef ANDREIUTILS_UTILSEIGENGEOMETRY_H
#define ANDREIUTILS_UTILSEIGENGEOMETRY_H

#include <Eigen/Dense>

namespace AndreiUtils {
    Eigen::Matrix3d xRotation(double angle);

    Eigen::Matrix3d yRotation(double angle);

    Eigen::Matrix3d zRotation(double angle);

    Eigen::Matrix4d quaternionConjugateDerivative();

    Eigen::Matrix4d quaternionProductDerivativeWRTFirst(const Eigen::Quaterniond &q1, const Eigen::Quaterniond &q2);

    Eigen::Matrix4d quaternionProductDerivativeWRTSecond(const Eigen::Quaterniond &q1, const Eigen::Quaterniond &q2);

    Eigen::Quaterniond quaternionFromEulerAnglesRotationOrderXYZ(const Eigen::Vector3d &euler);

    Eigen::Quaterniond quaternionFromEulerAnglesRotationOrderZYX(const Eigen::Vector3d &euler, bool componentOrderXYZ);

    Eigen::Matrix<double, 4, 3> quaternionDerivativeWithRespectToEulerAnglesRotationOrderXYZ(
            const Eigen::Vector3d &euler);

    Eigen::Matrix<double, 4, 3> quaternionDerivativeWithRespectToEulerAnglesRotationOrderZYX(
            const Eigen::Vector3d &euler, bool componentOrderXYZ);
}

#endif //ANDREIUTILS_UTILSEIGENGEOMETRY_H
