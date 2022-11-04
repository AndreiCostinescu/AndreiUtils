//
// Created by Andrei on 27-Feb-22.
//

#ifndef ANDREIUTILS_MOTIONDEVICEINTRINSICPARAMETERS_H
#define ANDREIUTILS_MOTIONDEVICEINTRINSICPARAMETERS_H

#include <Eigen/Dense>

namespace AndreiUtils {
    class MotionDeviceIntrinsicParameters {
    public:
        MotionDeviceIntrinsicParameters();

        MotionDeviceIntrinsicParameters(Eigen::MatrixXd data, Eigen::VectorXd biasVariances,
                                        Eigen::VectorXd noiseVariances);

        virtual ~MotionDeviceIntrinsicParameters();

        void setMotionDeviceIntrinsicParameters(Eigen::MatrixXd const &_data, Eigen::VectorXd const &_biasVariances,
                                                Eigen::VectorXd const &_noiseVariances);

        void setFromOther(MotionDeviceIntrinsicParameters const &other);

        void setFromOther(MotionDeviceIntrinsicParameters &&other);

        Eigen::MatrixXd data;
        Eigen::VectorXd biasVariances, noiseVariances;
    };
}

#endif //ANDREIUTILS_MOTIONDEVICEINTRINSICPARAMETERS_H
