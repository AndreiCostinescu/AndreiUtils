//
// Created by Andrei on 11.07.24.
//

#pragma once

#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsEigen.hpp>
#include <AndreiUtils/classes/RandomNumberGenerator.hpp>
#include <AndreiUtils/utilsEigenLeastSquares.h>
#include <cassert>
#include <Eigen/Dense>
#include <iostream>

namespace AndreiUtils {
    template<class T>
    Eigen::Matrix<T, 3, 1> xAxis3d() {
        const static Eigen::Matrix<T, 3, 1> a = Eigen::Matrix<T, 3, 1>::UnitX();
        return a;
    }

    template<class T>
    Eigen::Matrix<T, 3, 1> yAxis3d() {
        const static Eigen::Matrix<T, 3, 1> a = Eigen::Matrix<T, 3, 1>::UnitY();
        return a;
    }

    template<class T>
    Eigen::Matrix<T, 3, 1> zAxis3d() {
        const static Eigen::Matrix<T, 3, 1> a = Eigen::Matrix<T, 3, 1>::UnitZ();
        return a;
    }

    template<typename T, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    double vectorAngle(Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols> const &v1,
                       Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols> const &v2) {
        return acos(v1.normalized().dot(v2.normalized()));
    }

    template<typename T, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    double vectorAngleAlreadyNormalized(Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols> const &n1,
                                        Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols> const &n2) {
        return acos(n1.dot(n2));
    }

    template<typename T>
    bool checkInsideTriangles(
            Eigen::Matrix<T, 3, 1> const &p0, Eigen::Matrix<T, 3, -1> const &surfaceMatrix,
            Eigen::Matrix<T, -1, 3> const &otherShiftedPoints, Eigen::Index const &surfacePointCount,
            Eigen::Index const &otherPointCount, Eigen::Matrix<T, 3, -1> *otherSurfaceMatrix = nullptr,
            Eigen::Matrix<T, 3, 1> *otherProjectedP0 = nullptr, bool verbose = false) {
        bool withSettingOtherData = otherSurfaceMatrix != nullptr && otherProjectedP0 != nullptr;
        Eigen::Matrix3d leastSquaresMatrixA;
        leastSquaresMatrixA.col(2) = surfaceMatrix.col(surfacePointCount - 1);
        Eigen::Matrix3Xd coefficients;
        for (int surfaceVectorPairIndex = 0; surfaceVectorPairIndex < surfacePointCount - 2; ++surfaceVectorPairIndex) {
            leastSquaresMatrixA.template leftCols<2>() = surfaceMatrix.template middleCols<2>(surfaceVectorPairIndex);
            coefficients = AndreiUtils::leastSquares(leastSquaresMatrixA, otherShiftedPoints.transpose());
            if (verbose) {
                std::cout << "Coefficient matrix at surfaceVectorPairIndex " << surfaceVectorPairIndex << " ="
                          << std::endl << coefficients << std::endl;
            }
            for (int otherPointIndex = 0; otherPointIndex < otherPointCount; ++otherPointIndex) {
                if (inRange<double>(coefficients(0, otherPointIndex), 0, 1) &&
                    inRange<double>(coefficients(1, otherPointIndex), 0, 1) &&
                    inRange<double>(coefficients(0, otherPointIndex) + coefficients(1, otherPointIndex), 0, 1)) {
                    return true;
                }
                if (withSettingOtherData && surfaceVectorPairIndex == 0) {
                    coefficients(2, otherPointIndex) = 0;
                    if (otherPointIndex == 0) {
                        *otherProjectedP0 = leastSquaresMatrixA * coefficients.col(otherPointIndex);
                    } else {
                        otherSurfaceMatrix->col(otherPointIndex - 1) =
                                leastSquaresMatrixA * coefficients.col(otherPointIndex) - *otherProjectedP0;
                    }
                }
            }
        }
        if (withSettingOtherData) {
            // otherP0 should be p0 + surfaceMatrix * coefficients; but for computational efficiency, add p0 here
            *otherProjectedP0 += p0;
        }
        return false;
    }

    template<typename T>
    void getPerpendicularAxesFromOne(Eigen::Matrix<T, 3, 1> const &axis, Eigen::Matrix<T, 3, 1> &xAxis,
                                     Eigen::Matrix<T, 3, 1> &yAxis) {
        Eigen::Matrix<T, 3, 1> tmp(1, 0, 0);
        if (AndreiUtils::greaterEqual(AndreiUtils::fastAbs(axis.dot(tmp)), 0.9)) {
            // vectors are quasi-parallel
            tmp.x() = 0;
            tmp.y() = 1;
        }
        xAxis = axis.cross(tmp);
        yAxis = axis.cross(xAxis);
    }

    template<typename T>
    void getAnyOrientationFromOneAxis(Eigen::Matrix<T, 3, 1> axis, Eigen::Matrix<T, 3, 1> &xAxis,
                                      Eigen::Matrix<T, 3, 1> &yAxis, Eigen::Matrix<T, 3, 1> &zAxis,
                                      std::string const &whichAxis = "z") {
        if (whichAxis != "x" && whichAxis != "y" && whichAxis != "z") {
            throw std::runtime_error("Unknown axis type to process!");
        }
        axis.normalize();
        Eigen::Matrix<T, 3, 1> tmpAxis, tmpAxis2;
        getPerpendicularAxesFromOne(axis, tmpAxis, tmpAxis2);
        if (whichAxis == "z") {
            xAxis = tmpAxis;
            yAxis = tmpAxis2;
            zAxis = axis;
        } else if (whichAxis == "y") {
            zAxis = tmpAxis;
            xAxis = tmpAxis2;
            yAxis = axis;
        } else {
            assert(whichAxis == "x");
            yAxis = tmpAxis;
            zAxis = tmpAxis2;
            xAxis = axis;
        }
    }

    template<typename T>
    Eigen::Matrix<T, 3, 3> getAnyOrientationFromOneAxis(Eigen::Matrix<T, 3, 1> const &axis,
                                                        std::string const &whichAxis = "z") {
        Eigen::Matrix<T, 3, 1> x, y, z;
        getAnyOrientationFromOneAxis(axis, x, y, z, whichAxis);
        Eigen::Matrix<T, 3, 3> res;
        res.col(0) = x;
        res.col(1) = y;
        res.col(2) = z;
        return res;
    }

    template<typename T>
    Eigen::Quaternion<T> getAnyOrientationQuaternionFromOneAxis(Eigen::Matrix<T, 3, 1> const &axis,
                                                                std::string const &whichAxis = "z") {
        return Eigen::Quaternion<T>(getAnyOrientationFromOneAxis(axis, whichAxis));
    }

    template<typename T>
    void getOrientationFromTwoAxes(
            Eigen::Matrix<T, 3, 1> axis1, Eigen::Matrix<T, 3, 1> axis2, Eigen::Matrix<T, 3, 1> &xAxis,
            Eigen::Matrix<T, 3, 1> &yAxis, Eigen::Matrix<T, 3, 1> &zAxis, std::string const &whichAxis1 = "z",
            std::string const &whichAxis2 = "x") {
        if (whichAxis1 != "x" && whichAxis1 != "y" && whichAxis1 != "z") {
            throw std::runtime_error("Unknown axis1 type to process!");
        }
        if (whichAxis2 != "x" && whichAxis2 != "y" && whichAxis2 != "z") {
            throw std::runtime_error("Unknown axis2 type to process!");
        }
        axis1.normalize();
        axis2.normalize();
        Eigen::Matrix<T, 3, 1> tmpAxis;
        if (whichAxis1 == "z") {
            zAxis = axis1;
            if (whichAxis2 == "y") {
                xAxis = axis2.cross(zAxis);
                yAxis = zAxis.cross(xAxis);
            } else {
                yAxis = zAxis.cross(axis2);
                xAxis = yAxis.cross(zAxis);
            }
        } else if (whichAxis1 == "y") {
            yAxis = axis1;
            if (whichAxis2 == "x") {
                zAxis = axis2.cross(yAxis);
                xAxis = yAxis.cross(zAxis);
            } else {
                xAxis = yAxis.cross(axis2);
                zAxis = xAxis.cross(yAxis);
            }
        } else {
            assert(whichAxis1 == "x");
            xAxis = axis1;
            if (whichAxis2 == "z") {
                yAxis = axis2.cross(xAxis);
                zAxis = xAxis.cross(yAxis);
            } else {
                zAxis = xAxis.cross(axis2);
                yAxis = zAxis.cross(xAxis);
            }
        }
    }

    template<typename T>
    Eigen::Matrix<T, 3, 3> getOrientationFromTwoAxes(
            Eigen::Matrix<T, 3, 1> const &axis1, Eigen::Matrix<T, 3, 1> const &axis2,
            std::string const &whichAxis1 = "z", std::string const &whichAxis2 = "x") {
        Eigen::Matrix<T, 3, 1> x, y, z;
        getOrientationFromTwoAxes(axis1, axis2, x, y, z, whichAxis1, whichAxis2);
        Eigen::Matrix<T, 3, 3> res;
        res.col(0) = x;
        res.col(1) = y;
        res.col(2) = z;
        return res;
    }

    template<typename T>
    Eigen::Quaternion<T> getOrientationQuaternionFromTwoAxes(
            Eigen::Matrix<T, 3, 1> const &axis1, Eigen::Matrix<T, 3, 1> const &axis2,
            std::string const &whichAxis1 = "z", std::string const &whichAxis2 = "x") {
        return Eigen::Quaternion<T>(getOrientationFromTwoAxes(axis1, axis2, whichAxis1, whichAxis2));
    }

    template<typename T, int N>
    [[nodiscard]] Eigen::Matrix<T, N, 1> sampleUnitVector() {
        Eigen::Matrix<T, N, 1> res;
        for (int i = 0; i < N; ++i) {
            res(i) = double01Sampler.sample();
        }
        return addComponentWise(res, -0.5).normalized();
    }
}
