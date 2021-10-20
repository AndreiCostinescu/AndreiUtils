//
// Created by Andrei on 13.11.20.
//

#include <AndreiUtils/utilsOpenCVRealsense.h>
#include <iostream>

using namespace cv;
using namespace rs2;
using namespace std;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

// Convert frame to Mat
Mat AndreiUtils::frame_to_mat(const frame &f) {
    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    if (f.get_profile().format() == RS2_FORMAT_BGR8) {
        return {Size(w, h), CV_8UC3, (void *) f.get_data(), Mat::AUTO_STEP};
    } else if (f.get_profile().format() == RS2_FORMAT_RGB8) {
        auto r_rgb = Mat(Size(w, h), CV_8UC3, (void *) f.get_data(), Mat::AUTO_STEP);
        Mat r_bgr;
        cvtColor(r_rgb, r_bgr, COLOR_RGB2BGR);
        return r_bgr;
    } else if (f.get_profile().format() == RS2_FORMAT_Z16) {
        return {Size(w, h), CV_16UC1, (void *) f.get_data(), Mat::AUTO_STEP};
    } else if (f.get_profile().format() == RS2_FORMAT_Y8) {
        return {Size(w, h), CV_8UC1, (void *) f.get_data(), Mat::AUTO_STEP};
    } else if (f.get_profile().format() == RS2_FORMAT_DISPARITY32) {
        return {Size(w, h), CV_32FC1, (void *) f.get_data(), Mat::AUTO_STEP};
    }

    throw runtime_error("Frame format is not supported yet!");
}

#pragma clang diagnostic pop

// Converts depth frame to a matrix of doubles with distances in meters
Mat AndreiUtils::depth_frame_to_meters(const depth_frame &f) {
    Mat dm = frame_to_mat(f);
    dm.convertTo(dm, CV_64F, f.get_units());
    return dm;
}
