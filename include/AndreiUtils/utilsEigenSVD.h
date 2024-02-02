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
}

#endif //ANDREIUTILS_UTILSEIGENSVD_H
