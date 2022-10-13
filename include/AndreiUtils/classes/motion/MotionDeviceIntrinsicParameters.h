//
// Created by Andrei on 27-Feb-22.
//

#ifndef ANDREIUTILS_MOTIONDEVICEINTRINSICPARAMETERS_H
#define ANDREIUTILS_MOTIONDEVICEINTRINSICPARAMETERS_H

#include <AndreiUtils/json.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    class MotionDeviceInstrinsicParametersJsonSerializer;

    class MotionDeviceIntrinsicParameters {
    public:
        friend class MotionDeviceInstrinsicParametersJsonSerializer;

        MotionDeviceIntrinsicParameters();

        MotionDeviceIntrinsicParameters(Eigen::MatrixXd data, Eigen::VectorXd biasVariances,
                                        Eigen::VectorXd noiseVariances);

        virtual ~MotionDeviceIntrinsicParameters();

        void setMotionDeviceIntrinsicParameters(const Eigen::MatrixXd &_data, const Eigen::VectorXd &_biasVariances,
                                                const Eigen::VectorXd &_noiseVariances);
        Eigen::MatrixXd data;
        Eigen::VectorXd biasVariances, noiseVariances;
    };
}

#endif //ANDREIUTILS_MOTIONDEVICEINTRINSICPARAMETERS_H
