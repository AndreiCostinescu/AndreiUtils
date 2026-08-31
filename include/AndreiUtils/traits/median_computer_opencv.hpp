/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei on 20-Mar-22.
//

#ifndef ANDREIUTILS_MEDIAN_COMPUTER_OPENCV_HPP
#define ANDREIUTILS_MEDIAN_COMPUTER_OPENCV_HPP

#include <AndreiUtils/traits/median_computer.hpp>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    template<typename Scalar>
    struct median_computer<cv::Point_<Scalar>> {
        static cv::Point_<Scalar> medianComputer(std::vector<cv::Point_<Scalar>> v) {
            std::vector<Scalar> dimensionValues(v.size());
            cv::Point_<Scalar> res(0, 0);
            if (values.empty()) {
                return res;
            }
            Scalar value;
            for (int dim = 0; dim < 2; dim++) {
                for (int i = 0; i < values.size(); i++) {
                    if (dim == 0) {
                        value = values[i].x;
                    } else if (dim == 1) {
                        value = values[i].y;
                    }
                    dimensionValues[i] = value;
                }
                value = median_computer<Scalar>::medianComputer(dimensionValues);
                if (dim == 0) {
                    res.x = value;
                } else if (dim == 1) {
                    res.y = value;
                }
            }
            return res;
        }
    };

    template<typename Scalar>
    struct median_computer<cv::Point3_<Scalar>> {
        static cv::Point3_<Scalar> medianComputer(std::vector<cv::Point3_<Scalar>> v) {
            std::vector<Scalar> dimensionValues(v.size());
            cv::Point3_<Scalar> res(0, 0, 0);
            if (values.empty()) {
                return res;
            }
            Scalar value;
            for (int dim = 0; dim < 3; dim++) {
                for (int i = 0; i < values.size(); i++) {
                    if (dim == 0) {
                        value = values[i].x;
                    } else if (dim == 1) {
                        value = values[i].y;
                    } else if (dim == 2) {
                        value = values[i].z;
                    }
                    dimensionValues[i] = value;
                }
                value = median_computer<Scalar>::medianComputer(dimensionValues);
                if (dim == 0) {
                    res.x = value;
                } else if (dim == 1) {
                    res.y = value;
                } else if (dim == 2) {
                    res.z = value;
                }
            }
            return res;
        }
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_MEDIAN_COMPUTER_OPENCV_HPP
