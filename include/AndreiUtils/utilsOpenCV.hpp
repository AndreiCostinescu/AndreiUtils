//
// Created by andrei on 27.05.22.
//

#ifndef ANDREIUTILS_UTILSOPENCV_HPP
#define ANDREIUTILS_UTILSOPENCV_HPP

#include <functional>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    template<typename MatrixElementType, typename ReturnType>
    std::function<ReturnType(int, int)>
    getMatrixElementAccessor(cv::Mat const &m, ReturnType defaultValue = ReturnType()) {
        return [&m, defaultValue](int _x, int _y) {
            if (_x < 0 || _y < 0 || m.cols <= _x || m.rows <= _y) {
                return defaultValue;
            }
            return (ReturnType) m.template at<MatrixElementType>(_y, _x);
        };
    }

    template<typename MatrixElementType, typename ReturnType>
    std::function<ReturnType(int, int)>
    getMatrixElementAccessor(cv::Mat *const &m, ReturnType defaultValue = ReturnType()) {
        // std::cout << "Just Const Ref: m = " << m << std::endl;
        return [&m, defaultValue](int _x, int _y) {
            // std::cout << "In lambda Just Const Ref: m = " << m << std::endl;
            if (m == nullptr || _x < 0 || _y < 0 || m->cols <= _x || m->rows <= _y) {
                return defaultValue;
            }
            return (ReturnType) m->template at<MatrixElementType>(_y, _x);
        };
    }

    template<typename MatrixElementType, typename ReturnType>
    std::function<ReturnType(int, int)>
    getMatrixElementAccessor(cv::Mat const *const &m, ReturnType defaultValue = ReturnType()) {
        // std::cout << "Const Ptr Const Ref: m = " << m << std::endl;
        return [&m, defaultValue](int _x, int _y) {
            // std::cout << "In lambda Const Ptr Const Ref: m = " << m << std::endl;
            if (m == nullptr || _x < 0 || _y < 0 || m->cols <= _x || m->rows <= _y) {
                return defaultValue;
            }
            return (ReturnType) m->template at<MatrixElementType>(_y, _x);
        };
    }
}

#endif //ANDREIUTILS_UTILSOPENCV_HPP
