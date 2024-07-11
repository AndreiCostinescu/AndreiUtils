//
// Created by Andrei on 06.09.21.
//

#pragma once

#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsEigenLeastSquares.h>
#include <cassert>
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
}
