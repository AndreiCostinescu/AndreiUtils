//
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSEIGEN_H
#define ANDREIUTILS_UTILSEIGEN_H

#include <Eigen/Dense>
#include <string>
#include <vector>

namespace AndreiUtils {
    Eigen::MatrixXd readEigenData(const std::string &file);

    void writeEigenData(const std::string &file, const Eigen::MatrixXd &m);
}

#endif //ANDREIUTILS_UTILSEIGEN_H
