//
// Created by Andrei on 27.08.21.
//

#pragma once

#include <Eigen/Dense>
#include <string>
#include <vector>

namespace AndreiUtils {
    Eigen::MatrixXd readEigenData(std::string const &file);

    void writeEigenData(std::string const &file, const Eigen::MatrixXd &m);

    bool isOrthonormal(Eigen::Matrix3d const &m);
}
