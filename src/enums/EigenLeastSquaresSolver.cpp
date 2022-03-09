//
// Created by andrei on 08.03.22.
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
        default : {
            throw runtime_error("Unknown EigenLeastSquaresSolver " + to_string(solver));
        }
    }
}

EigenLeastSquaresSolver AndreiUtils::convertStringToEigenLeastSquaresSolver(const string &solver) {
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
