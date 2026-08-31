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

#include <AndreiUtils/enums/EigenSVDSolver.h>
#include <stdexcept>

using namespace AndreiUtils;
using namespace std;

std::string AndreiUtils::convertEigenSVDSolverToString(const EigenSVDSolver &solver) {
    switch (solver) {
        case JACOBI: {
            return "jacobi";
        }
        case BDC: {
            return "bdc";
        }
        default: {
            throw runtime_error("Unknown EigenSVDSolver " + to_string(solver));
        }
    }
}

EigenSVDSolver AndreiUtils::convertStringToEigenSVDSolver(string const &solver) {
    if (solver == "jacobi") {
        return AndreiUtils::EigenSVDSolver::JACOBI;
    } else if (solver == "bdc") {
        return AndreiUtils::EigenSVDSolver::BDC;
    } else {
        throw runtime_error("Unknown EigenSVDSolver: \"" + solver + "\"");
    }
}
