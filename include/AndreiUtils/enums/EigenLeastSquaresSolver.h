//
// Created by Andrei on 08.03.22.
//

#ifndef ANDREIUTILS_EIGENLEASTSQUARESSOLVER_H
#define ANDREIUTILS_EIGENLEASTSQUARESSOLVER_H

#include <string>

namespace AndreiUtils {
    enum EigenLeastSquaresSolver {
        NORMAL_EQUATIONS,
        QR_DECOMPOSITION,
        COMPUTE_ORTHOGONAL_DECOMPOSITION,
        SVD,
    };

    std::string convertEigenLeastSquaresSolverToString(const EigenLeastSquaresSolver &solver);

    EigenLeastSquaresSolver convertStringToEigenLeastSquaresSolver(const std::string &solver);
}

#endif //ANDREIUTILS_EIGENLEASTSQUARESSOLVER_H
