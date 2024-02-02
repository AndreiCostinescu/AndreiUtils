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
