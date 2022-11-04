//
// Created by Andrei on 21-Jan-22.
//

#include <AndreiUtils/classes/camera/ImageCaptureParameters.h>
#include <utility>

using namespace AndreiUtils;

ImageCaptureParameters::ImageCaptureParameters() : fps(), size() {}

ImageCaptureParameters::ImageCaptureParameters(double fps, ImageParameters const &size) : fps(fps), size(size) {}

ImageCaptureParameters::~ImageCaptureParameters() = default;

void ImageCaptureParameters::setFromOther(ImageCaptureParameters const &other) {
    if (this == &other) {
        return;
    }
    this->fps = other.fps;
    this->size = other.size;
}

void ImageCaptureParameters::setFromOther(ImageCaptureParameters &&other) {
    if (this == &other) {
        return;
    }
    this->fps = other.fps;
    this->size = std::move(other.size);
}
