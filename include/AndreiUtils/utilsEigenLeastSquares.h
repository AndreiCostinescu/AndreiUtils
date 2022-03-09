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
}

#endif //ANDREIUTILS_UTILSEIGENLEASTSQUARES_H
