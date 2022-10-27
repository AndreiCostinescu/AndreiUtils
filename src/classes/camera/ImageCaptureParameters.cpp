//
// Created by Andrei on 21-Jan-22.
//

#include <AndreiUtils/classes/camera/ImageCaptureParameters.h>

using namespace AndreiUtils;

ImageCaptureParameters::ImageCaptureParameters() : fps(), size() {}

ImageCaptureParameters::ImageCaptureParameters(double fps, ImageParameters const &size) : fps(fps), size(size) {}

ImageCaptureParameters::~ImageCaptureParameters() = default;
