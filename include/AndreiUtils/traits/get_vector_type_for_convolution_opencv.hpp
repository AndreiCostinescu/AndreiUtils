//
// Created by Andrei on 20-Mar-22.
//

#ifndef ANDREIUTILS_GET_VECTOR_TYPE_FOR_CONVOLUTION_OPENCV_HPP
#define ANDREIUTILS_GET_VECTOR_TYPE_FOR_CONVOLUTION_OPENCV_HPP

#include <AndreiUtils/traits/get_vector_type_for_convolution.hpp>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    template<typename Scalar>
    struct get_vector_type_for_convolution<cv::Point_<Scalar>> {
        typedef Scalar type;
    };

    template<typename Scalar>
    struct get_vector_type_for_convolution<cv::Point3_<Scalar>> {
        typedef Scalar type;
    };
}

#endif //ANDREIUTILS_GET_VECTOR_TYPE_FOR_CONVOLUTION_OPENCV_HPP
