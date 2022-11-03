//
// Created by Andrei on 21-Jan-22.
//

#include <AndreiUtils/classes/camera/ImageCaptureParametersWithIntrinsics.h>

using namespace AndreiUtils;

ImageCaptureParametersWithIntrinsics::ImageCaptureParametersWithIntrinsics() : ImageCaptureParameters(), intrinsics() {}

ImageCaptureParametersWithIntrinsics::ImageCaptureParametersWithIntrinsics(
        double fps, ImageParameters const &size, CameraIntrinsicParameters const &intrinsics) :
        ImageCaptureParameters(fps, size), intrinsics(intrinsics) {}

ImageCaptureParametersWithIntrinsics::~ImageCaptureParametersWithIntrinsics() = default;

void ImageCaptureParametersWithIntrinsics::setFromOther(ImageCaptureParametersWithIntrinsics const &other) {
    this->ImageCaptureParameters::setFromOther(other);
    this->intrinsics = other.intrinsics;
}
