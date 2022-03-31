//
// Created by andrei on 31.03.22.
//

#ifndef ANDREIUTILS_SET_ZERO_EIGEN_HPP
#define ANDREIUTILS_SET_ZERO_EIGEN_HPP

#include <AndreiUtils/traits/set_zero.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct set_zero<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static void setZero(Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &x) {
            x = Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>::Zero(x.size());
        }
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct set_zero<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static void setZero(Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &x) {
            x = Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>::Zero(x.size());
        }
    };
}

#endif //ANDREIUTILS_SET_ZERO_EIGEN_HPP
