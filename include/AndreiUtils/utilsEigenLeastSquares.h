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
// Created by Andrei on 15-Feb-22.
//

#ifndef ANDREIUTILS_UTILSEIGENLEASTSQUARES_H
#define ANDREIUTILS_UTILSEIGENLEASTSQUARES_H

#include <AndreiUtils/enums/EigenLeastSquaresSolver.h>
#include <Eigen/Dense>

namespace AndreiUtils {
    Eigen::MatrixXd leastSquares(const Eigen::MatrixXd &a, const Eigen::MatrixXd &b,
                                 EigenLeastSquaresSolver method = NORMAL_EQUATIONS);

    Eigen::MatrixXd leastSquaresCOD(const Eigen::MatrixXd &a, const Eigen::MatrixXd &b);

    Eigen::MatrixXd leastSquaresNormal(const Eigen::MatrixXd &a, const Eigen::MatrixXd &b);

    Eigen::MatrixXd leastSquaresQR(const Eigen::MatrixXd &a, const Eigen::MatrixXd &b);

    Eigen::MatrixXd leastSquaresSVD(const Eigen::MatrixXd &a, const Eigen::MatrixXd &b);
} // namespace AndreiUtils

#endif // ANDREIUTILS_UTILSEIGENLEASTSQUARES_H
