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

#include <AndreiUtils/utilsEigenLeastSquares.h>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

MatrixXd AndreiUtils::leastSquares(const MatrixXd &a, const MatrixXd &b, EigenLeastSquaresSolver method) {
    if (a.rows() != b.rows()) {
        throw runtime_error("Can't compute least squares from data which does not have the same amount of rows!");
    }
    switch (method) {
        case NORMAL_EQUATIONS: {
            return leastSquaresNormal(a, b);
        }
        case QR_DECOMPOSITION: {
            return leastSquaresQR(a, b);
        }
        case COMPUTE_ORTHOGONAL_DECOMPOSITION: {
            return leastSquaresCOD(a, b);
        }
        case SVD: {
            return leastSquaresSVD(a, b);
        }
        default: {
            throw runtime_error("Unknown least squares solving method!");
        }
    }
}

MatrixXd AndreiUtils::leastSquaresCOD(const MatrixXd &a, const MatrixXd &b) {
    if (a.rows() != b.rows()) {
        throw runtime_error("Can't compute least squares from data which does not have the same amount of rows!");
    }
    return a.completeOrthogonalDecomposition().solve(b);
}

MatrixXd AndreiUtils::leastSquaresNormal(const MatrixXd &a, const MatrixXd &b) {
    if (a.rows() != b.rows()) {
        throw runtime_error("Can't compute least squares from data which does not have the same amount of rows!");
    }
    return (a.transpose() * a).ldlt().solve(a.transpose() * b);
}

MatrixXd AndreiUtils::leastSquaresQR(const MatrixXd &a, const MatrixXd &b) {
    if (a.rows() != b.rows()) {
        throw runtime_error("Can't compute least squares from data which does not have the same amount of rows!");
    }
    return a.colPivHouseholderQr().solve(b);
}

MatrixXd AndreiUtils::leastSquaresSVD(const MatrixXd &a, const MatrixXd &b) {
    if (a.rows() != b.rows()) {
        throw runtime_error("Can't compute least squares from data which does not have the same amount of rows!");
    }
    // return a.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
    return leastSquaresCOD(a, b);
}
