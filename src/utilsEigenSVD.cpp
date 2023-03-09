//
// Created by Andrei on 20.01.23.
//

#include <AndreiUtils/utilsEigenSVD.h>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

Eigen::JacobiSVD<Eigen::MatrixXd> AndreiUtils::svdJacobi(Eigen::MatrixXd const &a, int computeOptions) {
    return a.jacobiSvd(computeOptions);
}

Eigen::BDCSVD<Eigen::MatrixXd> AndreiUtils::svdBDC(Eigen::MatrixXd const &a, int computeOptions) {
    return a.bdcSvd(computeOptions);
}