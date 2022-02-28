//
// Created by Andrei on 27-Feb-22.
//

#ifndef ANDREIUTILS_MOTIONDEVICECAPTUREPARAMETERS_H
#define ANDREIUTILS_MOTIONDEVICECAPTUREPARAMETERS_H

#include <AndreiUtils/json.hpp>
#include <AndreiUtils/classes/motion/MotionDeviceIntrinsicParameters.h>
#include <Eigen/Dense>

namespace AndreiUtils {
    class MotionDeviceCaptureParameters {
    public:
        MotionDeviceCaptureParameters();

        MotionDeviceCaptureParameters(const double &fps, const MotionDeviceIntrinsicParameters &intrinsics);

        virtual ~MotionDeviceCaptureParameters();

        void setMotionDeviceCaptureParameters(const double &_fps, const MotionDeviceIntrinsicParameters &_intrinsics);

        void toJson(nlohmann::json &j) const;

        void fromJson(const nlohmann::json &j);

        double fps;
        MotionDeviceIntrinsicParameters intrinsics;
    };
}

namespace nlohmann {
    template<>
    struct adl_serializer<AndreiUtils::MotionDeviceCaptureParameters> {
        static void to_json(nlohmann::json &j, const AndreiUtils::MotionDeviceCaptureParameters &p) {
            p.toJson(j);
        }

        static void from_json(const nlohmann::json &j, AndreiUtils::MotionDeviceCaptureParameters &p) {
            p.fromJson(j);
        }
    };
}

#endif //ANDREIUTILS_MOTIONDEVICECAPTUREPARAMETERS_H
