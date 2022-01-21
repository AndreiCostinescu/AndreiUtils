//
// Created by Andrei on 21-Jan-22.
//

#include <AndreiUtils/classes/camera/ImageParameters.h>

using namespace AndreiUtils;

ImageParameters::ImageParameters() : h(0), w(0) {}

ImageParameters::ImageParameters(int height, int width) : h(height), w(width) {}

ImageParameters::~ImageParameters() = default;

void ImageParameters::setImageParameters(int _h, int _w) {
    this->h = _h;
    this->w = _w;
}

void ImageParameters::toJson(nlohmann::json &j) const {
    j.clear();
    j["h"] = this->h;
    j["w"] = this->w;
}

void ImageParameters::fromJson(const nlohmann::json &j) {
    this->h = j.at("h").get<int>();
    this->w = j.at("w").get<int>();
}
