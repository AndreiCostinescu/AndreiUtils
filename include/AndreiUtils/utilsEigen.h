//
// Created by Andrei on 27.08.21.
//

#pragma once

#include <Eigen/Dense>
#include <string>
#include <vector>

namespace AndreiUtils {
    Eigen::MatrixXd readEigenData(const std::string &file);

    void writeEigenData(const std::string &file, const Eigen::MatrixXd &m);

    bool isOrthonormal(Eigen::Matrix3d const &m);
}
