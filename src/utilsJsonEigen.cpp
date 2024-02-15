//
// Created by Andrei on 15.02.24.
//

#include <AndreiUtils/utilsJsonEigen.hpp>

using namespace AndreiUtils;
using namespace nlohmann;

void adl_serializer<Symmetry>::to_json(json &j, Symmetry const &data) {
    j["type"] = data.type;
    std::vector<double> v(3);
    auto symmetryAxis = data.axis.normalized();
    for (int i = 0; i < 3; i++) {
        v[i] = symmetryAxis[i];
    }
    j["axis"] = v;
    for (int i = 0; i < 3; i++) {
        v[i] = data.axisDisplacementFromOrigin[i];
    }
    j["axisDisplacementFromOrigin"] = v;
    if (data.type == "rotation") {
        std::pair<double, double> degRange(AndreiUtils::rad2Deg(data.range.first),
                                           AndreiUtils::rad2Deg(data.range.second));
        AndreiUtils::bringValueInCircularInterval<double>(degRange.first, 0, 360);
        AndreiUtils::bringValueInCircularInterval<double>(degRange.second, 0, 360);
        j["range"] = degRange;
    } else {
        j["range"] = data.range;
    }
}

void adl_serializer<Symmetry>::from_json(json const &j, Symmetry &data) {
    data.type = j.at("type").get<std::string>();
    std::vector<double> v = j.at("axis").get<std::vector<double>>();
    for (int i = 0; i < 3; i++) {
        data.axis[i] = v[i];
    }
    data.axis.normalize();
    if (j.contains("axisDisplacementFromOrigin")) {
        v = j.at("axisDisplacementFromOrigin").get<std::vector<double>>();
        for (int i = 0; i < 3; i++) {
            data.axisDisplacementFromOrigin[i] = v[i];
        }
    }
    data.range = j.at("range").get<std::pair<double, double>>();
    if (data.type == "rotation") {
        AndreiUtils::bringValueInCircularInterval<double>(data.range.first, 0, 360);
        AndreiUtils::bringValueInCircularInterval<double>(data.range.second, 0, 360);
        data.range.first = AndreiUtils::deg2Rad(data.range.first);
        data.range.second = AndreiUtils::deg2Rad(data.range.second);
    }
}

void adl_serializer<MotionDeviceIntrinsicParameters>::to_json(json &j, MotionDeviceIntrinsicParameters const &data) {
    j.clear();
    j["data"] = data.data;
    j["biasVariances"] = data.biasVariances;
    j["noiseVariances"] = data.noiseVariances;
}

void adl_serializer<MotionDeviceIntrinsicParameters>::from_json(json const &j, MotionDeviceIntrinsicParameters &data) {
    data.data = j.at("data").get<Eigen::MatrixXd>();
    data.biasVariances = j.at("biasVariances").get<Eigen::VectorXd>();
    data.noiseVariances = j.at("noiseVariances").get<Eigen::VectorXd>();
}

void adl_serializer<MotionDeviceCaptureParameters>::to_json(json &j, MotionDeviceCaptureParameters const &data) {
    j.clear();
    j["fps"] = data.fps;
    j["intrinsics"] = data.intrinsics;
}

void adl_serializer<MotionDeviceCaptureParameters>::from_json(json const &j, MotionDeviceCaptureParameters &data) {
    data.fps = j.at("fps").get<double>();
    data.intrinsics = j.at("intrinsics").get<AndreiUtils::MotionDeviceIntrinsicParameters>();
}

void adl_serializer<Grasp>::to_json(json &j, Grasp const &data, bool withCurrentPose) {
    j.clear();
    j["points"] = data.originalGraspPointPoses;
    if (withCurrentPose) {
        j["currentPoints"] = data.graspPointPoses;
        j["relativeTo"] = data.relativeTo;
    }
}

void adl_serializer<Grasp>::from_json(json const &j, Grasp &data, bool withCurrentPose) {
    data.originalGraspPointPoses = j.at("points").get<std::vector<AndreiUtils::Posed>>();
    if (withCurrentPose) {
        data.graspPointPoses = j.at("currentPoints").get<std::vector<Posed>>();
        data.relativeTo = j.at("relativeTo").get<Posed>();
    } else {
        data.graspPointPoses = data.originalGraspPointPoses;
        data.relativeTo = Posed::one;
    }
}

/*
void adl_serializer<GraspWithAngleRange>::to_json(json &j, GraspWithAngleRange const &data) {
    adl_serializer<Grasp>::to_json(j, data);
    if (data.hasGraspAngleRange) {
        std::pair<double, double> degAngleRange(AndreiUtils::rad2Deg(data.graspAngleRange.first),
                                                AndreiUtils::rad2Deg(data.graspAngleRange.second));
        j["angleRange"] = degAngleRange;
    }
}

void adl_serializer<GraspWithAngleRange>::from_json(json const &j, GraspWithAngleRange &data) {
    adl_serializer<Grasp>::from_json(j, data);
    data.hasGraspAngleRange = j.contains("angleRange");
    if (data.hasGraspAngleRange) {
        data.graspAngleRange = j.at("angleRange").get<std::pair<double, double>>();
        data.graspAngleRange.first = AndreiUtils::deg2Rad(data.graspAngleRange.first);
        data.graspAngleRange.second = AndreiUtils::deg2Rad(data.graspAngleRange.second);
    }
}
//*/
