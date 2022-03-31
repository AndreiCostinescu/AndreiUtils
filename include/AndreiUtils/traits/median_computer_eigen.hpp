//
// Created by Andrei on 20-Mar-22.
//

#ifndef ANDREIUTILS_MEDIAN_COMPUTER_EIGEN_HPP
#define ANDREIUTILS_MEDIAN_COMPUTER_EIGEN_HPP

#include <AndreiUtils/traits/median_computer.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct median_computer<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> medianComputer(
                std::vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> v) {
            std::vector<Scalar> dimensionValues(v.size());
            Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> res;
            res.setZero();
            if (v.empty()) {
                return res;
            }
            const Eigen::Index rows = v[0].rows(), cols = v[0].cols();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    for (int k = 0; k < v.size(); k++) {
                        dimensionValues[k] = v[k](i, j);
                    }
                    res(i, j) = median_computer<Scalar>::medianComputer(dimensionValues);
                }
            }
            return res;
        }
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct median_computer<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> medianComputer(
                std::vector<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> v) {
            std::vector<Scalar> dimensionValues(v.size());
            Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> res;
            res.setZero();
            if (v.empty()) {
                return res;
            }
            const Eigen::Index rows = v[0].rows(), cols = v[0].cols();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    for (int k = 0; k < v.size(); k++) {
                        dimensionValues[k] = v[k](i, j);
                    }
                    res(i, j) = median_computer<Scalar>::medianComputer(dimensionValues);
                }
            }
            return res;
        }
    };
}

#endif //ANDREIUTILS_MEDIAN_COMPUTER_EIGEN_HPP
