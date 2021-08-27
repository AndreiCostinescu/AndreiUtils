//
// Created by andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSEIGEN_H
#define ANDREIUTILS_UTILSEIGEN_H

#include <string>
#include <Eigen/Dense>

std::string eigenToString(const Eigen::MatrixXd &eigenData);

bool checkSize(const Eigen::MatrixXd &a, const Eigen::MatrixXd &b);

#endif //ANDREIUTILS_UTILSEIGEN_H
