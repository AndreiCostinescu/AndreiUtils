//
// Created by Andrei on 26.11.20.
//

#pragma once

#include <AndreiUtils/classes/camera/CameraIntrinsicParameters.h>
#include <AndreiUtils/classes/camera/ImageParameters.h>
#include <AndreiUtils/enums/ImageDistortionModel.h>
#include <cstdint>
#include <librealsense2/rs.hpp>

namespace AndreiUtils {
    // Convert rs2::frame to data
    void frameToBytes(const rs2::frame &f, uint8_t *data, int &dataType, size_t dataElements);

    // Converts depth frame to a double data with distances in meters
    void depthFrameToMeters(const rs2::depth_frame &f, double *data, size_t dataElements);

    // Converts depth frame to a uint16_t data with distances in meters
    void depthFrameToMilliMeters(const rs2::depth_frame &f, uint16_t *data, size_t dataElements);

    // Returns the 3D point at the requested (x, y) position of the depth data
    void getRealsenseDepthPointFromImagePixel(std::function<float(int, int)> &getDepth, rs2_intrinsics &intrinsics,
                                              float x, float y, float (&point)[3], int windowSize = 1,
                                              bool forceWindowUsage = false, float farthestAllowedDepth = 10);

    // Returns the image pixel at the requested 3D position
    void getImagePixelFromRealsenseDepthPoint(rs2_intrinsics *intrinsics, float point[3], float (&pixel)[2]);

    rs2_intrinsics convertCameraIntrinsicParametersToRealsenseIntrinsics(
            const ImageParameters &size, const CameraIntrinsicParameters &intrinsics);

    void convertRealsenseIntrinsicsToCameraIntrinsicParameters(
            const rs2_intrinsics &rsIntrinsics, ImageParameters &size, CameraIntrinsicParameters &intrinsics);

    ImageDistortionModel convertRealsenseDistortionToImageDistortionModel(const rs2_distortion &distortion);

    rs2_distortion convertImageDistortionModelToRealsenseDistortion(const ImageDistortionModel &distortion);
}
