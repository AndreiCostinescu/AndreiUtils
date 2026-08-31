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

#ifndef ANDREIUTILS_CONTAINER2DEIGEN_HPP
#define ANDREIUTILS_CONTAINER2DEIGEN_HPP

#include <AndreiUtils/traits/Container2D.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    class Container2D<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
    public:
        static const bool isContainer2D = true;

        explicit Container2D(const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &data) : data(data) {}

        inline int height() const { return this->data.rows(); }

        inline int width() const { return this->data.cols(); }

        Scalar valueAt(int row, int col) const { return this->data(row, col); }

        bool checkPointInsideContainer(int row, int col) const {
            return 0 <= row && row < this->height() && 0 <= col && col < this->width();
        }

        bool checkPointInsideContainer(double row, double col) const {
            return 0 <= row && row < this->height() && 0 <= col && col < this->width();
        }

    protected:
        const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &data;
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    class ModifiableContainer2D<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>>
        : public Container2D<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
    public:
        explicit ModifiableContainer2D(Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &data) :
            Container2D<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>>(data), modifiableData(data) {}

        void setValueAt(int row, int col, const Scalar &value) { this->modifiabledata(row, col) = value; }

    protected:
        Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &modifiableData;
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_CONTAINER2DEIGEN_HPP
