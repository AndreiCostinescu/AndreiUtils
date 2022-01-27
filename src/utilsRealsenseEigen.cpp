//
// Created by andrei on 26.01.22.
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

rs2_motion_device_intrinsic AndreiUtils::convertEigenParametersToRealsenseMotionIntrinsics(
        const MatrixXd &data, const VectorXd &biasVariances, const VectorXd &noiseVariances) {
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
