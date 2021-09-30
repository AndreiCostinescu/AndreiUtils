//
// Created by Andrei on 17.09.21.
//

#include <AndreiUtils/utilsEigenGeometry.h>

using namespace Eigen;

Matrix3d AndreiUtils::xRotation(double angle) {
    Matrix3d x;
    x << 1, 0, 0, 0, cos(angle), -sin(angle), 0, sin(angle), cos(angle);
    return x;
}

Matrix3d AndreiUtils::yRotation(double angle) {
    Matrix3d y;
    y << cos(angle), 0, sin(angle), 0, 1, 0, -sin(angle), 0, cos(angle);
    return y;
}

Matrix3d AndreiUtils::zRotation(double angle) {
    Matrix3d z;
    z << cos(angle), -sin(angle), 0, sin(angle), cos(angle), 0, 0, 0, 1;
    return z;
}

Quaterniond AndreiUtils::quaternionFromEulerAnglesRotationOrderXYZ(const Vector3d &euler) {
    double cx = cos(euler(0) / 2);
    double sx = sin(euler(0) / 2);
    double cy = cos(euler(1) / 2);
    double sy = sin(euler(1) / 2);
    double cz = cos(euler(2) / 2);
    double sz = sin(euler(2) / 2);

    Quaterniond q;
    q.w() = cx * cy * cz - sx * sy * sz;
    q.x() = sx * cy * cz + cx * sy * sz;
    q.y() = cx * sy * cz - sx * cy * sz;
    q.z() = cx * cy * sz + sx * sy * cz;
    return q;
}

Matrix<double, 4, 3> AndreiUtils::quaternionDerivativeWithRespectToEulerAnglesRotationOrderXYZ(const Vector3d &euler) {
    double cx = cos(euler(0) / 2);
    double sx = sin(euler(0) / 2);
    double cy = cos(euler(1) / 2);
    double sy = sin(euler(1) / 2);
    double cz = cos(euler(2) / 2);
    double sz = sin(euler(2) / 2);

    Matrix<double, 4, 3> J;
    J(0, 0) = -sx * cy * cz - cx * sy * sz;
    J(0, 1) = -cx * sy * cz - sx * cy * sz;
    J(0, 2) = -cx * cy * sz - sx * sy * cz;

    J(1, 0) = cx * cy * cz - sx * sy * sz;
    J(1, 1) = -sx * sy * cz + cx * cy * sz;
    J(1, 2) = -sx * cy * cz + cx * sy * sz;

    J(2, 0) = -sx * sy * cz - cx * cy * sz;
    J(2, 1) = cx * cy * cz + sx * sy * sz;
    J(2, 2) = -cx * sy * sz - sx * cy * cz;

    J(3, 0) = -sx * cy * sz + cx * sy * cz;
    J(3, 1) = - cx * sy * sz + sx * cy * cz;
    J(3, 2) = -cx * cy * cz - sx * sy * sz;

    return J / 2;
}

Quaterniond AndreiUtils::quaternionFromEulerAnglesRotationOrderZYX(const Vector3d &euler, bool componentOrderXYZ) {
    int xIndex = 2 - 2 * componentOrderXYZ;
    int yIndex = 1;
    int zIndex = 2 * componentOrderXYZ;
    double cz = cos(euler(zIndex) / 2);
    double sz = sin(euler(zIndex) / 2);
    double cy = cos(euler(yIndex) / 2);
    double sy = sin(euler(yIndex) / 2);
    double cx = cos(euler(xIndex) / 2);
    double sx = sin(euler(xIndex) / 2);

    Quaterniond q;
    q.w() = cx * cy * cz + sx * sy * sz;
    q.x() = sx * cy * cz - cx * sy * sz;
    q.y() = cx * sy * cz + sx * cy * sz;
    q.z() = cx * cy * sz - sx * sy * cz;
    return q;
}

Matrix<double, 4, 3> AndreiUtils::quaternionDerivativeWithRespectToEulerAnglesRotationOrderZYX(const Vector3d &euler,
                                                                                               bool componentOrderXYZ) {
    int xIndex = 2 - 2 * componentOrderXYZ;
    int yIndex = 1;
    int zIndex = 2 * componentOrderXYZ;
    double cz = cos(euler(zIndex) / 2);
    double sz = sin(euler(zIndex) / 2);
    double cy = cos(euler(yIndex) / 2);
    double sy = sin(euler(yIndex) / 2);
    double cx = cos(euler(xIndex) / 2);
    double sx = sin(euler(xIndex) / 2);

    Matrix<double, 4, 3> J;
    J(0, zIndex) = -cx * cy * sz + sx * sy * cz;
    J(0, yIndex) = -cx * sy * cz + sx * cy * sz;
    J(0, xIndex) = -sx * cy * cz + cx * sy * sz;

    J(1, zIndex) = -sx * cy * sz - cx * sy * cz;
    J(1, yIndex) = -sx * sy * cz - cx * cy * sz;
    J(1, xIndex) = cx * cy * cz + sx * sy * sz;

    J(2, zIndex) = -cx * sy * sz + sx * cy * cz;
    J(2, yIndex) = cx * cy * cz - sx * sy * sz;
    J(2, xIndex) = -sx * sy * cz + cx * cy * sz;

    J(3, zIndex) = cx * cy * cz + sx * sy * sz;
    J(3, yIndex) = -cx * sy * sz - sx * cy * cz;
    J(3, xIndex) = -sx * cy * sz - cx * sy * cz;

    return J / 2;
}
