//
// Created by Andrei on 20.01.23.
//

#ifndef ANDREIUTILS_EIGENLEASTSQUARESSOLVER_H
#define ANDREIUTILS_EIGENLEASTSQUARESSOLVER_H

#include <string>

namespace AndreiUtils {
    enum EigenSVDSolver {
        JACOBI,
        BDC,
    };

    std::string convertEigenSVDSolverToString(EigenSVDSolver const &solver);

    EigenSVDSolver convertStringToEigenSVDSolver(std::string const &solver);
}

#endif //ANDREIUTILS_EIGENLEASTSQUARESSOLVER_H
