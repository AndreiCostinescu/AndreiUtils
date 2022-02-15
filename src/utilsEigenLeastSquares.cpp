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
        default : {
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