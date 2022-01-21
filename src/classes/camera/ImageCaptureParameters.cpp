//
// Created by Andrei on 21-Jan-22.
//

#include <AndreiUtils/classes/camera/ImageCaptureParameters.h>

using namespace AndreiUtils;

ImageCaptureParameters::ImageCaptureParameters() : fps(), size() {}

ImageCaptureParameters::ImageCaptureParameters(double fps, const ImageParameters &size) : fps(fps), size(size) {}

ImageCaptureParameters::~ImageCaptureParameters() = default;

void ImageCaptureParameters::toJson(nlohmann::json &j) const {
    j.clear();
    j["fps"] = this->fps;
    // this->size.toJson(j["size"]);
    j["size"] = this->size;
}

void ImageCaptureParameters::fromJson(const nlohmann::json &j) {
    this->fps = j.at("fps").get<double>();
    // this->size.from_json(j.at("size"));
    this->size = j.at("size").get<ImageParameters>();
}
