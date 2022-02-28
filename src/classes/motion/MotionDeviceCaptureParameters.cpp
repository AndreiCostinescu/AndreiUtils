//
// Created by Andrei on 27-Feb-22.
//

#include <AndreiUtils/classes/motion/MotionDeviceCaptureParameters.h>

using namespace AndreiUtils;
using namespace std;

MotionDeviceCaptureParameters::MotionDeviceCaptureParameters() : fps(), intrinsics() {}

MotionDeviceCaptureParameters::MotionDeviceCaptureParameters(
        const double &fps, const MotionDeviceIntrinsicParameters &intrinsics) : fps(fps), intrinsics(intrinsics) {}

MotionDeviceCaptureParameters::~MotionDeviceCaptureParameters() = default;

void MotionDeviceCaptureParameters::setMotionDeviceCaptureParameters(
        const double &_fps, const MotionDeviceIntrinsicParameters &_intrinsics) {
    this->fps = _fps;
    this->intrinsics = _intrinsics;
}

void MotionDeviceCaptureParameters::toJson(nlohmann::json &j) const {
    j.clear();
    j["fps"] = this->fps;
    j["intrinsics"] = this->intrinsics;
}

void MotionDeviceCaptureParameters::fromJson(const nlohmann::json &j) {
    this->fps = j.at("fps").get<double>();
    this->intrinsics = j.at("intrinsics").get<MotionDeviceIntrinsicParameters>();
}
