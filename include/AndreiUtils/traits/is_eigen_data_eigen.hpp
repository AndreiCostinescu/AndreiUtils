//
// Created by andrei on 31.03.22.
//

#ifndef ANDREIUTILS_IS_EIGEN_DATA_EIGEN_HPP
#define ANDREIUTILS_IS_EIGEN_DATA_EIGEN_HPP

#include <AndreiUtils/traits/is_eigen_data.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct is_eigen_data<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static const bool value = true;
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct is_eigen_data<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static const bool value = true;
    };
}

#endif //ANDREIUTILS_IS_EIGEN_DATA_EIGEN_HPP
