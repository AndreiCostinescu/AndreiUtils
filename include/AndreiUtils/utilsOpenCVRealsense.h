/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#pragma once

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp> // Include OpenCV API

namespace AndreiUtils {
    // Wrap rs2::frame as cv::Mat
    cv::Mat frame_to_mat(const rs2::frame &f);

    // Convert rs2::frame to cv::Mat (allocates new data)
    cv::Mat convertFrameToMat(const rs2::frame &f);

    // Wraps depth frame as a matrix of doubles with distances in meters
    cv::Mat depth_frame_to_meters(const rs2::depth_frame &f);

    // Converts depth frame to a matrix of doubles with distances in meters (allocates new data)
    cv::Mat convertDepthFrameToMeters(const rs2::depth_frame &f);
} // namespace AndreiUtils
