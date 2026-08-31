// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 27.05.22.
//

#ifndef ANDREIUTILS_UTILSOPENCV_HPP
#define ANDREIUTILS_UTILSOPENCV_HPP

#include <AndreiUtils/utils.hpp>
#include <functional>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    template<typename MatrixElementType, typename ReturnType>
    std::function<ReturnType(int, int)> getMatrixElementAccessor(cv::Mat const &m,
                                                                 ReturnType defaultValue = ReturnType()) {
        return [m, defaultValue](int _x, int _y) {
            if (_x < 0 || _y < 0 || m.cols <= _x || m.rows <= _y) {
                return defaultValue;
            }
            return (ReturnType) m.template at<MatrixElementType>(_y, _x);
        };
    }

    template<typename MatrixElementType, typename ReturnType>
    std::function<ReturnType(int, int)> getMatrixElementAccessor(cv::Mat *const &m,
                                                                 ReturnType defaultValue = ReturnType()) {
        // std::cout << "Just Const Ref: m = " << m << std::endl;
        return [m, defaultValue](int _x, int _y) {
            // std::cout << "In lambda Just Const Ref: m = " << m << std::endl;
            if (m == nullptr || _x < 0 || _y < 0 || m->cols <= _x || m->rows <= _y) {
                return defaultValue;
            }
            return (ReturnType) m->template at<MatrixElementType>(_y, _x);
        };
    }

    template<typename MatrixElementType, typename ReturnType>
    std::function<ReturnType(int, int)> getMatrixElementAccessor(cv::Mat const *const &m,
                                                                 ReturnType defaultValue = ReturnType()) {
        // std::cout << "Const Ptr Const Ref: m = " << m << std::endl;
        return [m, defaultValue](int _x, int _y) {
            // std::cout << "In lambda Const Ptr Const Ref: m = " << m << std::endl;
            if (m == nullptr || _x < 0 || _y < 0 || m->cols <= _x || m->rows <= _y) {
                return defaultValue;
            }
            return (ReturnType) m->template at<MatrixElementType>(_y, _x);
        };
    }

    template<typename MatrixElementType, typename ReturnType>
    std::function<ReturnType(int, int)>
    getMatrixElementAccessorWithReferenceToData(cv::Mat const &m, ReturnType defaultValue = ReturnType()) {
        return [&m, defaultValue](int _x, int _y) {
            if (_x < 0 || _y < 0 || m.cols <= _x || m.rows <= _y) {
                return defaultValue;
            }
            return (ReturnType) m.template at<MatrixElementType>(_y, _x);
        };
    }

    template<typename MatrixElementType, typename ReturnType>
    std::function<ReturnType(int, int)>
    getMatrixElementAccessorWithReferenceToData(cv::Mat *const &m, ReturnType defaultValue = ReturnType()) {
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
    getMatrixElementAccessorWithReferenceToData(cv::Mat const *const &m, ReturnType defaultValue = ReturnType()) {
        // std::cout << "Const Ptr Const Ref: m = " << m << std::endl;
        return [&m, defaultValue](int _x, int _y) {
            // std::cout << "In lambda Const Ptr Const Ref: m = " << m << std::endl;
            if (m == nullptr || _x < 0 || _y < 0 || m->cols <= _x || m->rows <= _y) {
                return defaultValue;
            }
            return (ReturnType) m->template at<MatrixElementType>(_y, _x);
        };
    }

    template<typename T>
    cv::Point_<T> clampX(cv::Point_<T> const &p, T const &min, T const &max) {
        return {AndreiUtils::clamp<T>(p.x, min, max), p.y};
    }

    template<typename T>
    cv::Point_<T> clampY(cv::Point_<T> const &p, T const &min, T const &max) {
        return {p.x, AndreiUtils::clamp<T>(p.y, min, max)};
    }
} // namespace AndreiUtils

#endif // ANDREIUTILS_UTILSOPENCV_HPP
