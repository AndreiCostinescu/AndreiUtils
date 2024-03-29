//
// Created by Andrei on 13.11.20.
//

#include <AndreiUtils/utilsRealsense.h>
#include <AndreiUtils/utils.hpp>
#include <cstring>
#include <librealsense2/rsutil.h>
#include <iostream>

using namespace rs2;
using namespace std;

int convertFrameTypeToDataType(const size_t elementSize, const int nrChannels) {
    int dataType = (nrChannels - 1) << 3;
    if (elementSize == 1) {
        dataType += 0;
    } else if (elementSize == 2) {
        dataType += 2;
    } else if (elementSize == 4) {
        dataType += 5;
    }
    return dataType;
}

void AndreiUtils::frameToBytes(const rs2::frame &f, uint8_t **data, int &h, int &w, int &c, StandardTypes &dataType,
                               bool copyData) {
    // This could be replaced by making data a reference pointer (uint8_t *&data), but casting would become problematic
    if (data == nullptr || (copyData && *data == nullptr)) {
        throw runtime_error("Data container of realsense-frame-data is nullptr...");
    }

    auto vf = f.as<video_frame>();
    w = vf.get_width();
    h = vf.get_height();

    size_t elementSize;
    if (f.get_profile().format() == RS2_FORMAT_BGR8 || f.get_profile().format() == RS2_FORMAT_RGB8) {
        elementSize = sizeof(uint8_t);
        dataType = StandardTypes::TYPE_UINT_8;
        c = 3;
    } else if (f.get_profile().format() == RS2_FORMAT_Z16) {
        elementSize = sizeof(uint16_t);
        dataType = StandardTypes::TYPE_UINT_16;
        c = 1;
    } else if (f.get_profile().format() == RS2_FORMAT_Y8) {
        elementSize = sizeof(uint8_t);
        dataType = StandardTypes::TYPE_UINT_8;
        c = 1;
    } else if (f.get_profile().format() == RS2_FORMAT_DISPARITY32) {
        elementSize = sizeof(uint32_t);
        dataType = StandardTypes::TYPE_UINT_32;
        c = 1;
    } else {
        throw runtime_error("Frame format is not supported yet!");
    }

    // size_t frameElements = w * h * c;
    if (vf.get_bytes_per_pixel() != c * elementSize) {
        size_t nrBytes = w * h * c * elementSize;
        int dataElements = w * h * vf.get_bytes_per_pixel();
        throw runtime_error("The container data and the frame data do not have the same amount of bytes: " +
                            to_string(dataElements) + " vs. " + to_string(nrBytes));
    }
    if (copyData) {
        size_t nrBytes = w * h * c * elementSize;
        fastMemCopy(*data, (const uint8_t *) f.get_data(), nrBytes);
        if (f.get_profile().format() == RS2_FORMAT_BGR8) {
            // Switch BGR to RGB format
            fastForLoop<uint8_t>(*data, nrBytes, [](uint8_t *const array, size_t i, size_t increment) {
                swapData(array[i], array[i + 2]);
            }, 3);
        }
    } else {
        *data = (uint8_t *) f.get_data();
    }
}

void AndreiUtils::frameToBytes(const rs2::frame &f, uint8_t **data, int &dataType, const size_t dataElements) {
    int h, w, c, elementSize;
    StandardTypes type;
    frameToBytes(f, data, h, w, c, type, true);
    elementSize = getStandardTypeByteAmount(type);
    if (dataElements != h * w * c * elementSize) {
        throw runtime_error("The container data and the frame data do not have the same amount of bytes: " +
                            to_string(dataElements) + " vs. " + to_string(h * w * c * elementSize));
    }
    dataType = convertFrameTypeToDataType(elementSize, c);
}

void AndreiUtils::depthFrameToMeters(const rs2::depth_frame &f, double *&data, const size_t dataElements) {
    auto *tmpData = new uint16_t[dataElements];
    int dataType;
    frameToBytes(f, (uint8_t **) &tmpData, dataType, dataElements * sizeof(uint16_t));
    assert (dataType == convertFrameTypeToDataType(2, 1));
    if (data != nullptr) {
        fastSrcOp<uint16_t, double>(data, (uint16_t *) tmpData, dataElements,
                                    [&](const uint16_t &x) { return (double) x * f.get_units(); });
    }
    delete[] tmpData;
}

void AndreiUtils::depthFrameToMilliMeters(const rs2::depth_frame &f, uint16_t *&data, const size_t dataElements) {
    int dataType;
    frameToBytes(f, (uint8_t **) &data, dataType, dataElements * sizeof(uint16_t));
    assert (dataType == convertFrameTypeToDataType(2, 1));
    if (data != nullptr) {
        fastSrcOp<uint16_t, uint16_t>(data, data, dataElements, [&](const uint16_t &x) {
            return (uint16_t) ((double) x * f.get_units() * 1000);
        });
    }
}

void AndreiUtils::fromImagePixelTo3dPoint(const rs2_intrinsics &intrinsics, const float position[2], float depth,
                                          float (&point)[3]) {
    rs2_deproject_pixel_to_point(point, &intrinsics, position, depth);
}

void AndreiUtils::fromImagePixelTo3dPoint(const rs2_intrinsics &intrinsics, float x, float y, float depth,
                                          float (&point)[3]) {
    float position[2];
    position[0] = x;
    position[1] = y;
    fromImagePixelTo3dPoint(intrinsics, position, depth, point);
}

void AndreiUtils::getRealsenseDepthPointFromImagePixel(
        const function<float(int, int)> &getDepth, const rs2_intrinsics &intrinsics, float x, float y,
        float (&point)[3], int windowSize, bool forceWindowUsage, float farthestAllowedDepth) {
    int width = intrinsics.width, height = intrinsics.height;
    int int_x = int(x), int_y = int(y);
    float position[2] = {x, y};
    // rs2_deproject_pixel_to_point(point, &intrinsics, position, getDepth(int_x, int_y));
    // return;

    float depth = getDepth(int_x, int_y), avgDepth = 0;
    auto isDepthInvalid = [farthestAllowedDepth](float x) {
        return isnan(x) || lessEqual(x, 0.1f) || lessEqual(farthestAllowedDepth, x);
    };
    if (forceWindowUsage || isDepthInvalid(depth)) {
        int nrPoints = 0;
        for (int i = max(-int_x, -windowSize); i <= min(windowSize, height - int_x - 1); i++) {
            for (int j = max(-int_y, -windowSize); j <= min(windowSize, width - int_y - 1); j++) {
                depth = getDepth(int_x + i, int_y + j);
                if (isDepthInvalid(depth)) {
                    continue;
                }
                avgDepth += depth;
                nrPoints += 1;
            }
        }
        if (nrPoints < 1) {
            // cerr << "Can not compute depth average from less than one point..." << endl;
            avgDepth = 0;
        } else {
            avgDepth /= (float) nrPoints;
        }
    } else {
        avgDepth = depth;
    }

    // De-project from pixel to point in 3D: Get the distance at the given pixel
    fromImagePixelTo3dPoint(intrinsics, position, avgDepth, point);
}

void AndreiUtils::getRealsenseDepthPointFromImagePixel(
        const function<float(int, int)> &getDepth, const ImageParameters &size,
        const CameraIntrinsicParameters &intrinsics, float x, float y, float (&point)[3], int windowSize,
        bool forceWindowUsage, float farthestAllowedDepth) {
    return getRealsenseDepthPointFromImagePixel(
            getDepth, convertCameraIntrinsicParametersToRealsenseIntrinsics(size, intrinsics), x, y, point, windowSize,
            forceWindowUsage, farthestAllowedDepth);
}

void AndreiUtils::getImagePixelFromRealsenseDepthPoint(rs2_intrinsics const *intrinsics, float point[3], float (&pixel)[2]) {
    // project pixel from point in 3D
    rs2_project_point_to_pixel(pixel, intrinsics, point);
}

void AndreiUtils::getImagePixelFromRealsenseDepthPoint(
        const ImageParameters &size, const CameraIntrinsicParameters &intrinsics, float point[3], float (&pixel)[2]) {
    rs2_intrinsics i = convertCameraIntrinsicParametersToRealsenseIntrinsics(size, intrinsics);
    getImagePixelFromRealsenseDepthPoint(&i, point, pixel);
}

rs2_intrinsics AndreiUtils::convertCameraIntrinsicParametersToRealsenseIntrinsics(
        const AndreiUtils::ImageParameters &size, const AndreiUtils::CameraIntrinsicParameters &intrinsics) {
    rs2_intrinsics i;
    i.width = size.w;
    i.height = size.h;
    i.fx = (float) intrinsics.fx;
    i.fy = (float) intrinsics.fy;
    i.ppx = (float) intrinsics.ppx;
    i.ppy = (float) intrinsics.ppy;
    i.ppy = (float) intrinsics.ppy;
    i.model = convertImageDistortionModelToRealsenseDistortion(intrinsics.distortionModel);
    for (int index = 0; index < intrinsics.nrDistortionCoefficients; index++) {
        i.coeffs[index] = intrinsics.distortionCoefficients[index];
    }
    return i;
}

void AndreiUtils::convertRealsenseIntrinsicsToCameraIntrinsicParameters(
        const rs2_intrinsics &rsIntrinsics, AndreiUtils::ImageParameters &size,
        AndreiUtils::CameraIntrinsicParameters &intrinsics) {
    size.setImageParameters(rsIntrinsics.height, rsIntrinsics.width);
    intrinsics.setImageParameters(rsIntrinsics.fx, rsIntrinsics.fy, rsIntrinsics.ppx, rsIntrinsics.ppy);
    intrinsics.setDistortionParameters(convertRealsenseDistortionToImageDistortionModel(rsIntrinsics.model),
                                       rsIntrinsics.coeffs);
}

AndreiUtils::ImageDistortionModel AndreiUtils::convertRealsenseDistortionToImageDistortionModel(
        const rs2_distortion &distortion) {
    switch (distortion) {
        case RS2_DISTORTION_NONE: {
            return ImageDistortionModel::DISTORTION_NONE;
        }
        case RS2_DISTORTION_MODIFIED_BROWN_CONRADY: {
            return ImageDistortionModel::DISTORTION_MODIFIED_BROWN_CONRADY;
        }
        case RS2_DISTORTION_INVERSE_BROWN_CONRADY: {
            return ImageDistortionModel::DISTORTION_INVERSE_BROWN_CONRADY;
        }
        case RS2_DISTORTION_FTHETA: {
            return ImageDistortionModel::DISTORTION_FTHETA;
        }
        case RS2_DISTORTION_BROWN_CONRADY: {
            return ImageDistortionModel::DISTORTION_BROWN_CONRADY;
        }
        case RS2_DISTORTION_KANNALA_BRANDT4: {
            return ImageDistortionModel::DISTORTION_KANNALA_BRANDT4;
        }
        case RS2_DISTORTION_COUNT: {
            throw runtime_error("RS2_DISTORTION_COUNT is not a valid distortion model");
        }
        default: {
            throw runtime_error("Unknown rs2_distortion type: " + to_string(distortion));
        }
    }
}

rs2_distortion AndreiUtils::convertImageDistortionModelToRealsenseDistortion(const ImageDistortionModel &distortion) {
    switch (distortion) {
        case ImageDistortionModel::DISTORTION_NONE: {
            return RS2_DISTORTION_NONE;
        }
        case ImageDistortionModel::DISTORTION_MODIFIED_BROWN_CONRADY: {
            return RS2_DISTORTION_MODIFIED_BROWN_CONRADY;
        }
        case ImageDistortionModel::DISTORTION_INVERSE_BROWN_CONRADY: {
            return RS2_DISTORTION_INVERSE_BROWN_CONRADY;
        }
        case ImageDistortionModel::DISTORTION_FTHETA: {
            return RS2_DISTORTION_FTHETA;
        }
        case ImageDistortionModel::DISTORTION_BROWN_CONRADY: {
            return RS2_DISTORTION_BROWN_CONRADY;
        }
        case ImageDistortionModel::DISTORTION_KANNALA_BRANDT4: {
            return RS2_DISTORTION_KANNALA_BRANDT4;
        }
        default: {
            throw runtime_error("Unknown or incompatible distortion type: " + to_string(distortion));
        }
    }
}
