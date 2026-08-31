/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei on 26.01.22.
//

#include <AndreiUtils/utilsRealsenseEigen.h>

using namespace Eigen;

void AndreiUtils::convertRealsenseMotionIntrinsicsToEigenParameters(const rs2_motion_device_intrinsic &motionIntrinsics,
                                                                    MatrixXd &data, VectorXd &biasVariances,
                                                                    VectorXd &noiseVariances) {
    data = MatrixXd::Zero(3, 4);
    biasVariances = VectorXd::Zero(3);
    noiseVariances = VectorXd::Zero(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            data(i, j) = motionIntrinsics.data[i][j];
        }
        biasVariances(i) = motionIntrinsics.bias_variances[i];
        noiseVariances(i) = motionIntrinsics.noise_variances[i];
    }
}

rs2_motion_device_intrinsic
AndreiUtils::convertEigenParametersToRealsenseMotionIntrinsics(const MatrixXd &data, const VectorXd &biasVariances,
                                                               const VectorXd &noiseVariances) {
    rs2_motion_device_intrinsic motionIntrinsics{};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            motionIntrinsics.data[i][j] = (float) data(i, j);
        }
        motionIntrinsics.bias_variances[i] = (float) biasVariances(i);
        motionIntrinsics.noise_variances[i] = (float) noiseVariances(i);
    }
    return motionIntrinsics;
}
