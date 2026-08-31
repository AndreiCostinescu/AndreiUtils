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

#ifndef ANDREIUTILS_CONTAINER2DOPENCV_HPP
#define ANDREIUTILS_CONTAINER2DOPENCV_HPP

#include <AndreiUtils/traits/Container2D.hpp>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    template<typename Scalar>
    class TypeMat : public cv::Mat {
    public:
        explicit TypeMat(const cv::Mat &mat) : cv::Mat(mat) {}

        Scalar const &atType(int row, int col) const {
            return static_cast<cv::Mat const &>(*this).at<Scalar>(row, col);
        }

        Scalar &atType(int row, int col) { return static_cast<cv::Mat &>(*this).at<Scalar>(row, col); }
    };

    template<typename Scalar>
    class Container2D<TypeMat<Scalar>> {
    public:
        static const bool isContainer2D = true;

        explicit Container2D(const TypeMat<Scalar> &data) : data(data) {}

        inline int height() const { return this->data.rows; }

        inline int width() const { return this->data.cols; }

        Scalar valueAt(int row, int col) const { return this->data.atType(row, col); }

        bool checkPointInsideContainer(int row, int col) const {
            return 0 <= row && row < this->height() && 0 <= col && col < this->width();
        }

        bool checkPointInsideContainer(double row, double col) const {
            return 0 <= row && row < this->height() && 0 <= col && col < this->width();
        }

    protected:
        const TypeMat<Scalar> &data;
    };

    template<typename Scalar>
    class ModifiableContainer2D<TypeMat<Scalar>> : public Container2D<TypeMat<Scalar>> {
    public:
        explicit ModifiableContainer2D(TypeMat<Scalar> &data) : Container2D<TypeMat<Scalar>>(data), data(data) {}

        void setValueAt(int row, int col, const Scalar &value) { this->modifiabledata.atType(row, col) = value; }

    protected:
        const TypeMat<Scalar> &data;
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_CONTAINER2DOPENCV_HPP
