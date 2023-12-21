//
// Created by Andrei on 06.09.21.
//

#pragma once

#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsEigenLeastSquares.h>
#include <Eigen/Dense>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace AndreiUtils {
    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    std::string eigenToString(Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> const &eigenData) {
        std::stringstream ss;
        ss << eigenData;
        return ss.str();
    }

    template<typename S1, int R1, int C1, int O1, int MR1, int MC1, typename S2, int R2, int C2, int O2, int MR2, int MC2>
    bool checkSameSize(Eigen::Matrix<S1, R1, C1, O1, MR1, MC1> const &a,
                       Eigen::Matrix<S2, R2, C2, O2, MR2, MC2> const &b) {
        return a.rows() == b.rows() && a.cols() == b.cols();
    }

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    std::vector<Eigen::Matrix<Scalar, Cols, 1, Options, MaxCols, 1>> getMatrixRowsAsVector(
            Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> const &m) {
        std::vector<Eigen::Matrix<Scalar, Cols, 1, Options, MaxCols, 1>> res(m.rows());
        for (int i = 0; i < m.rows(); i++) {
            res[i] = m.row(i);
        }
        return res;
    }

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    std::vector<Eigen::Matrix<Scalar, Rows, 1, Options, MaxRows, 1>> getMatrixColsAsVector(
            Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> const &m) {
        std::vector<Eigen::Matrix<Scalar, Rows, 1, Options, MaxRows, 1>> res(m.cols());
        for (int i = 0; i < m.cols(); i++) {
            res[i] = m.col(i);
        }
        return res;
    }

    // https://gist.github.com/javidcf/25066cf85e71105d57b6
    template<class MatT>
    Eigen::Matrix<typename MatT::Scalar, MatT::ColsAtCompileTime, MatT::RowsAtCompileTime> pseudoinverse(
            MatT const &mat, typename MatT::Scalar tolerance = typename MatT::Scalar{1e-4}) {
        typedef typename MatT::Scalar Scalar;
        auto svd = mat.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV);
        const auto &singularValues = svd.singularValues();
        Eigen::Matrix<Scalar, MatT::ColsAtCompileTime, MatT::RowsAtCompileTime> singularValuesInv(mat.cols(),
                                                                                                  mat.rows());
        singularValuesInv.setZero();
        for (unsigned int i = 0; i < singularValues.size(); ++i) {
            if (singularValues(i) > tolerance) {
                singularValuesInv(i, i) = Scalar{1} / singularValues(i);
            } else {
                singularValuesInv(i, i) = Scalar{0};
            }
        }
        return svd.matrixV() * singularValuesInv * svd.matrixU().adjoint();
    }

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
    Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols> addComponentWise(
            Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols> const &m, T const &v) {
        return m.array() + v;
    }

    template<typename T, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols> subComponentWise(
            Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols> const &m, T const &v) {
        return m.array() - v;
    }

    template<class T, int N>
    Eigen::Matrix<T, -1, N> convertVectorsToMatrixRows(std::vector<Eigen::Matrix<T, N, 1>> const &rows) {
        Eigen::Matrix<T, -1, N> m = Eigen::Matrix<T, -1, N>::Zero(rows.size(), N);
        for (int i = 0; i < m.rows(); i++) {
            m.row(i) = rows[i];
        }
        return m;
    }

    template<class T>
    Eigen::Matrix<T, -1, 1> convertValuesToVector(std::vector<T> const &values) {
        Eigen::Matrix<T, -1, 1> v = Eigen::Matrix<T, -1, 1>::Zero(values.size(), 1);
        for (int i = 0; i < values.size(); i++) {
            v[i] = values[i];
        }
        return v;
    }

    template<class T>
    Eigen::Matrix<T, 1, -1> convertValuesToRowVector(std::vector<T> const &values) {
        Eigen::Matrix<T, 1, -1> v = Eigen::Matrix<T, 1, -1>::Zero(1, values.size());
        for (int i = 0; i < values.size(); i++) {
            v[i] = values[i];
        }
        return v;
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
    void getAnyOrientationFromOneAxis(Eigen::Matrix<T, 3, 1> const &axis, Eigen::Matrix<T, 3, 1> &xAxis,
                                      Eigen::Matrix<T, 3, 1> &yAxis, Eigen::Matrix<T, 3, 1> &zAxis,
                                      std::string const &whichAxis = "z") {
        if (whichAxis != "x" && whichAxis != "y" && whichAxis != "z") {
            throw std::runtime_error("Unknown axis type to process!");
        }
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
}
