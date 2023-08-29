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
        default : {
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
