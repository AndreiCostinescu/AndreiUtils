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
// Created by Andrei on 26.01.22.
//

#ifndef ANDREIUTILS_UTILSREALSENSEEIGEN_H
#define ANDREIUTILS_UTILSREALSENSEEIGEN_H

#include <Eigen/Dense>
#include <librealsense2/rs.hpp>

namespace AndreiUtils {
    void convertRealsenseMotionIntrinsicsToEigenParameters(const rs2_motion_device_intrinsic &motionIntrinsics,
                                                           Eigen::MatrixXd &data, Eigen::VectorXd &biasVariances,
                                                           Eigen::VectorXd &noiseVariances);

    rs2_motion_device_intrinsic
    convertEigenParametersToRealsenseMotionIntrinsics(const Eigen::MatrixXd &data, const Eigen::VectorXd &biasVariances,
                                                      const Eigen::VectorXd &noiseVariances);
} // namespace AndreiUtils

#endif // ANDREIUTILS_UTILSREALSENSEEIGEN_H
