// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#pragma once

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API

namespace AndreiUtils {
    // Convert rs2::frame to cv::Mat
    cv::Mat frame_to_mat(const rs2::frame &f);

    // Converts depth frame to a matrix of doubles with distances in meters
    cv::Mat depth_frame_to_meters(const rs2::depth_frame &f);
}
