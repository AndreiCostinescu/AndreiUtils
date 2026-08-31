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
// Created by Andrei on 31.03.22.
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
} // namespace AndreiUtils

#endif // ANDREIUTILS_IS_EIGEN_DATA_EIGEN_HPP
