//
// Created by Andrei on 20-Mar-22.
//

#ifndef ANDREIUTILS_GET_VECTOR_TYPE_FOR_CONVOLUTION_EIGEN_HPP
#define ANDREIUTILS_GET_VECTOR_TYPE_FOR_CONVOLUTION_EIGEN_HPP

#include <AndreiUtils/traits/get_vector_type_for_convolution.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct get_vector_type_for_convolution<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        typedef Scalar type;
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct get_vector_type_for_convolution<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        typedef Scalar type;
    };
}

#endif //ANDREIUTILS_GET_VECTOR_TYPE_FOR_CONVOLUTION_EIGEN_HPP
