// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
} // namespace AndreiUtils
