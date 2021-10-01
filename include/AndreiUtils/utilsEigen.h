//
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSEIGEN_H
#define ANDREIUTILS_UTILSEIGEN_H

#include <Eigen/Dense>
#include <string>

namespace AndreiUtils {
    std::string eigenToString(const Eigen::MatrixXd &eigenData);

    bool checkSameSize(const Eigen::MatrixXd &a, const Eigen::MatrixXd &b);

    Eigen::MatrixXd readEigenData(const std::string &file);

    void writeEigenData(const std::string &file, const Eigen::MatrixXd &m);
}

#endif //ANDREIUTILS_UTILSEIGEN_H
