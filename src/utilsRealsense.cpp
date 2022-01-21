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
    int dataType = 3 * (nrChannels - 1);
    if (elementSize == 1) {
        dataType += 0;
    } else if (elementSize == 2) {
        dataType += 2;
    } else if (elementSize == 4) {
        dataType += 5;
    }
    return dataType;
}

void AndreiUtils::frameToBytes(const rs2::frame &f, uint8_t *data, int &dataType, const size_t dataElements) {
    if (data == nullptr) {
        throw runtime_error("Data container of realsense-frame-data is nullptr...");
    }

    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    size_t elementSize;
    int nrChannels;
    if (f.get_profile().format() == RS2_FORMAT_BGR8 || f.get_profile().format() == RS2_FORMAT_RGB8) {
        elementSize = sizeof(uint8_t);
        nrChannels = 3;
    } else if (f.get_profile().format() == RS2_FORMAT_Z16) {
        elementSize = sizeof(uint16_t);
        nrChannels = 1;
    } else if (f.get_profile().format() == RS2_FORMAT_Y8) {
        elementSize = sizeof(uint8_t);
        nrChannels = 1;
    } else if (f.get_profile().format() == RS2_FORMAT_DISPARITY32) {
        elementSize = sizeof(uint32_t);
        nrChannels = 1;
    } else {
        throw runtime_error("Frame format is not supported yet!");
    }

    size_t frameElements = w * h * nrChannels;
    if (dataElements != frameElements * elementSize) {
        throw runtime_error("The container data and the frame data do not have the same amount of elements: " +
                            to_string(dataElements) + " vs. " + to_string(frameElements));
    }
    memcpy(data, f.get_data(), frameElements * elementSize);
    if (f.get_profile().format() == RS2_FORMAT_BGR8) {
        size_t nrBytes = frameElements * elementSize;
        for (size_t i = 0; i < nrBytes; i += 3) {
            // Switch BGR to RGB format
            swap(data[i], data[i + 2]);
        }
    }
    dataType = convertFrameTypeToDataType(elementSize, nrChannels);
}

void AndreiUtils::depthFrameToMeters(const rs2::depth_frame &f, double *data, const size_t dataElements) {
    auto *tmpData = new uint16_t[dataElements];
    int dataType;
    frameToBytes(f, (uint8_t *) tmpData, dataType, dataElements * sizeof(uint16_t));
    assert (dataType == convertFrameTypeToDataType(2, 1));
    fastSrcOp<uint16_t, double>(data, tmpData, dataElements, [](const uint16_t &x) { return (double) (x / 1000.0); });
    delete[] tmpData;
}

void AndreiUtils::depthFrameToMilliMeters(const rs2::depth_frame &f, uint16_t *data, const size_t dataElements) {
    int dataType;
    frameToBytes(f, (uint8_t *) data, dataType, dataElements * sizeof(uint16_t));
    assert (dataType == convertFrameTypeToDataType(2, 1));
}

void AndreiUtils::getRealsenseDepthPointFromImagePixel(function<float(int, int)> &getDepth, rs2_intrinsics &intrinsics,
                                                       float x, float y, float (&point)[3], int windowSize,
                                                       bool forceWindowUsage, float farthestAllowedDepth) {
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
    rs2_deproject_pixel_to_point(point, &intrinsics, position, avgDepth);
}

void AndreiUtils::getImagePixelFromRealsenseDepthPoint(rs2_intrinsics *intrinsics, float point[3], float (&pixel)[2]) {
    // project pixel from point in 3D
    rs2_project_point_to_pixel(pixel, intrinsics, point);
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
