//
// Created by Andrei on 17.09.21.
//

#include <AndreiUtils/utilsEigenGeometry.h>

using namespace Eigen;

Matrix3d xRotation(double angle) {
    Matrix3d x;
    x << 1, 0, 0, 0, cos(angle), -sin(angle), 0, sin(angle), cos(angle);
    return x;
}

Matrix3d yRotation(double angle) {
    Matrix3d y;
    y << cos(angle), 0, sin(angle), 0, 1, 0, -sin(angle), 0, cos(angle);
    return y;
}

Matrix3d zRotation(double angle) {
    Matrix3d z;
    z << cos(angle), -sin(angle), 0, sin(angle), cos(angle), 0, 0, 0, 1;
    return z;
}
