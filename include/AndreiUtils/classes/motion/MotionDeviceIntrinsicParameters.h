//
// Created by Andrei on 27-Feb-22.
//

#ifndef ANDREIUTILS_MOTIONDEVICEINTRINSICPARAMETERS_H
#define ANDREIUTILS_MOTIONDEVICEINTRINSICPARAMETERS_H

#include <AndreiUtils/json.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    class MotionDeviceIntrinsicParameters {
    public:
        MotionDeviceIntrinsicParameters();

        MotionDeviceIntrinsicParameters(Eigen::MatrixXd data, Eigen::VectorXd biasVariances,
                                        Eigen::VectorXd noiseVariances);

        virtual ~MotionDeviceIntrinsicParameters();

        void setMotionDeviceIntrinsicParameters(const Eigen::MatrixXd &_data, const Eigen::VectorXd &_biasVariances,
                                                const Eigen::VectorXd &_noiseVariances);

        void toJson(nlohmann::json &j) const;

        void fromJson(const nlohmann::json &j);

        Eigen::MatrixXd data;
        Eigen::VectorXd biasVariances, noiseVariances;
    };
}

namespace nlohmann {
    template<>
    struct adl_serializer<AndreiUtils::MotionDeviceIntrinsicParameters> {
        static void to_json(nlohmann::json &j, const AndreiUtils::MotionDeviceIntrinsicParameters &p) {
            p.toJson(j);
        }

        static void from_json(const nlohmann::json &j, AndreiUtils::MotionDeviceIntrinsicParameters &p) {
            p.fromJson(j);
        }
    };
}

#endif //ANDREIUTILS_MOTIONDEVICEINTRINSICPARAMETERS_H
