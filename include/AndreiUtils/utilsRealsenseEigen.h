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

    rs2_motion_device_intrinsic convertEigenParametersToRealsenseMotionIntrinsics(
            const Eigen::MatrixXd &data, const Eigen::VectorXd &biasVariances, const Eigen::VectorXd &noiseVariances);
}

#endif //ANDREIUTILS_UTILSREALSENSEEIGEN_H
