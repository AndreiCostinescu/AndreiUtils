// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#pragma once

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include <exception>
#include <enums/DepthDataType.h>

// Convert rs2::frame to cv::Mat
cv::Mat frame_to_mat(const rs2::frame &f);

// Converts depth frame to a matrix of doubles with distances in meters
cv::Mat depth_frame_to_meters(const rs2::depth_frame &f);

// Returns the 3D point at the requested (x, y) position of the depth frame
void getRealsenseDepthPointFromImagePixel(void *depthData, DepthDataType dataType, float x, float y, float (&point)[3],
                                          int windowSize = 1, bool forceWindowUsage = false);

// Returns the image pixel at the requested 3D position
void getImagePixelFromRealsenseDepthPoint(rs2_intrinsics *intrinsics, float point[3], float (&pixel)[2]);
