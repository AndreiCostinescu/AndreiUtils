//
// Created by Andrei on 27-Feb-22.
//

#ifndef ANDREIUTILS_MOTIONDEVICECAPTUREPARAMETERS_H
#define ANDREIUTILS_MOTIONDEVICECAPTUREPARAMETERS_H

#include <AndreiUtils/classes/motion/MotionDeviceIntrinsicParameters.h>
#include <Eigen/Dense>

namespace AndreiUtils {
    class MotionDeviceCaptureParametersJsonSerializer;

    class MotionDeviceCaptureParameters {
    public:
        friend class MotionDeviceCaptureParametersJsonSerializer;

        MotionDeviceCaptureParameters();

        MotionDeviceCaptureParameters(const double &fps, const MotionDeviceIntrinsicParameters &intrinsics);

        virtual ~MotionDeviceCaptureParameters();

        void setMotionDeviceCaptureParameters(const double &_fps, const MotionDeviceIntrinsicParameters &_intrinsics);

        double fps;
        MotionDeviceIntrinsicParameters intrinsics;
    };
}

#endif //ANDREIUTILS_MOTIONDEVICECAPTUREPARAMETERS_H
