//
// Created by Andrei on 21-Jan-22.
//

#include <AndreiUtils/classes/camera/ImageCaptureParametersWithIntrinsics.h>

using namespace AndreiUtils;

ImageCaptureParametersWithIntrinsics::ImageCaptureParametersWithIntrinsics() : ImageCaptureParameters(), intrinsics() {}

ImageCaptureParametersWithIntrinsics::ImageCaptureParametersWithIntrinsics(
        double fps, const ImageParameters &size, const CameraIntrinsicParameters &intrinsics) :
        ImageCaptureParameters(fps, size), intrinsics(intrinsics) {}

ImageCaptureParametersWithIntrinsics::~ImageCaptureParametersWithIntrinsics() = default;

void ImageCaptureParametersWithIntrinsics::toJson(nlohmann::json &j) const {
    ImageCaptureParameters::toJson(j);
    j["intrinsics"] = this->intrinsics;
}

void ImageCaptureParametersWithIntrinsics::fromJson(const nlohmann::json &j) {
    this->intrinsics = j.at("intrinsics").get<CameraIntrinsicParameters>();
    ImageCaptureParameters::fromJson(j);
}
