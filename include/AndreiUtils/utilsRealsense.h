// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#pragma once

#include <cstdint>
#include <librealsense2/rs.hpp>

namespace AndreiUtils {
    // Convert rs2::frame to data
    void frameToBytes(const rs2::frame &f, uint8_t *&data, int &dataType, int dataElements);

    // Converts depth frame to a double data with distances in meters
    void depthFrameToMeters(const rs2::depth_frame &f, double *data, int dataElements);

    // Returns the 3D point at the requested (x, y) position of the depth data
    void getRealsenseDepthPointFromImagePixel(std::function<float(int, int)> &getDepth, rs2_intrinsics &intrinsics,
                                              float x, float y, float (&point)[3], int windowSize = 1,
                                              bool forceWindowUsage = false);

    // Returns the image pixel at the requested 3D position
    void getImagePixelFromRealsenseDepthPoint(rs2_intrinsics *intrinsics, float point[3], float (&pixel)[2]);
}
