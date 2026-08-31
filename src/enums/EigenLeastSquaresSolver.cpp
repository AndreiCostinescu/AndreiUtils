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
// Created by Andrei on 08.03.22.
//

#include <AndreiUtils/enums/EigenLeastSquaresSolver.h>
#include <stdexcept>

using namespace AndreiUtils;
using namespace std;

std::string AndreiUtils::convertEigenLeastSquaresSolverToString(const EigenLeastSquaresSolver &solver) {
    switch (solver) {
        case NORMAL_EQUATIONS: {
            return "normal_equations";
        }
        case QR_DECOMPOSITION: {
            return "qr_decomposition";
        }
        case COMPUTE_ORTHOGONAL_DECOMPOSITION: {
            return "orthogonal_decomposition";
        }
        case SVD: {
            return "svd";
        }
        default: {
            throw runtime_error("Unknown EigenLeastSquaresSolver " + to_string(solver));
        }
    }
}

EigenLeastSquaresSolver AndreiUtils::convertStringToEigenLeastSquaresSolver(string const &solver) {
    if (solver == "normal_equations") {
        return AndreiUtils::EigenLeastSquaresSolver::NORMAL_EQUATIONS;
    } else if (solver == "qr_decomposition") {
        return AndreiUtils::EigenLeastSquaresSolver::QR_DECOMPOSITION;
    } else if (solver == "orthogonal_decomposition") {
        return AndreiUtils::EigenLeastSquaresSolver::COMPUTE_ORTHOGONAL_DECOMPOSITION;
    } else if (solver == "svd") {
        return AndreiUtils::EigenLeastSquaresSolver::SVD;
    } else {
        throw runtime_error("Unknown EigenLeastSquaresSolver: \"" + solver + "\"");
    }
}
