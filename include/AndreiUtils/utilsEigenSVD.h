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
// Created by Andrei on 20.01.23.
//

#ifndef ANDREIUTILS_UTILSEIGENSVD_H
#define ANDREIUTILS_UTILSEIGENSVD_H

#include <AndreiUtils/enums/EigenSVDSolver.h>
#include <Eigen/Dense>

namespace AndreiUtils {
    Eigen::JacobiSVD<Eigen::MatrixXd> svdJacobi(Eigen::MatrixXd const &a, int computeOptions);

    Eigen::BDCSVD<Eigen::MatrixXd> svdBDC(Eigen::MatrixXd const &a, int computeOptions);
} // namespace AndreiUtils

#endif // ANDREIUTILS_UTILSEIGENSVD_H
