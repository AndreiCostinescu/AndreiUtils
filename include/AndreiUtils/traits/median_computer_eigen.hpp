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
// Created by Andrei on 20-Mar-22.
//

#ifndef ANDREIUTILS_MEDIAN_COMPUTER_EIGEN_HPP
#define ANDREIUTILS_MEDIAN_COMPUTER_EIGEN_HPP

#include <AndreiUtils/traits/median_computer.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct median_computer<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>
        medianComputer(std::vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> v) {
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
        static Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>
        medianComputer(std::vector<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> v) {
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
} // namespace AndreiUtils

#endif // ANDREIUTILS_MEDIAN_COMPUTER_EIGEN_HPP
