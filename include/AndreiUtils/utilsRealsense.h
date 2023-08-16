//
// Created by Andrei on 26.11.20.
//

#pragma once

#include <AndreiUtils/classes/camera/CameraIntrinsicParameters.h>
#include <AndreiUtils/classes/camera/ImageParameters.h>
#include <AndreiUtils/enums/ImageDistortionModel.h>
#include <AndreiUtils/enums/StandardTypes.h>
#include <cstdint>
#include <librealsense2/rs.hpp>

namespace AndreiUtils {
    // Convert rs2::frame to data
    void frameToBytes(rs2::frame const &f, uint8_t **data, int &h, int &w, int &c, StandardTypes &dataType,
                      bool copyData);

    // Convert rs2::frame to data
    void frameToBytes(rs2::frame const &f, uint8_t **data, int &dataType, size_t dataElements);

    // Converts depth frame to a double data with distances in meters
    void depthFrameToMeters(rs2::depth_frame const &f, double *&data, size_t dataElements);

    // Converts depth frame to a uint16_t data with distances in millimeters
    void depthFrameToMilliMeters(rs2::depth_frame const &f, uint16_t *&data, size_t dataElements);

    void fromImagePixelTo3dPoint(rs2_intrinsics const &intrinsics, float const position[2], float depth,
                                 float (&point)[3]);

    void fromImagePixelTo3dPoint(rs2_intrinsics const &intrinsics, float x, float y, float depth, float (&point)[3]);

    // Returns the 3D point at the requested (x, y) position of the depth data
    void getRealsenseDepthPointFromImagePixel(
            std::function<float(int, int)> const &getDepth, rs2_intrinsics const &intrinsics, float x, float y,
            float (&point)[3], int windowSize = 1, bool forceWindowUsage = false, float farthestAllowedDepth = 10);

    void getRealsenseDepthPointFromImagePixel(
            std::function<float(int, int)> const &getDepth, AndreiUtils::ImageParameters const &size,
            AndreiUtils::CameraIntrinsicParameters const &intrinsics, float x, float y, float (&point)[3],
            int windowSize = 1, bool forceWindowUsage = false, float farthestAllowedDepth = 10);

    // Returns the image pixel at the requested 3D position
    void getImagePixelFromRealsenseDepthPoint(rs2_intrinsics const *intrinsics, float point[3], float (&pixel)[2]);

    void getImagePixelFromRealsenseDepthPoint(
            AndreiUtils::ImageParameters const &size, AndreiUtils::CameraIntrinsicParameters const &intrinsics,
            float point[3], float (&pixel)[2]);

    rs2_intrinsics convertCameraIntrinsicParametersToRealsenseIntrinsics(
            ImageParameters const &size, CameraIntrinsicParameters const &intrinsics);

    void convertRealsenseIntrinsicsToCameraIntrinsicParameters(
            rs2_intrinsics const &rsIntrinsics, ImageParameters &size, CameraIntrinsicParameters &intrinsics);

    ImageDistortionModel convertRealsenseDistortionToImageDistortionModel(rs2_distortion const &distortion);

    rs2_distortion convertImageDistortionModelToRealsenseDistortion(ImageDistortionModel const &distortion);

    rs2_option getRealsenseOptionFromString(std::string const &option);
}
