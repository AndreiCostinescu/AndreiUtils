//
// Created by andrei on 13.11.20.
//

#include <andrei_utils/utilsRealsense.h>
#include <librealsense2/rsutil.h>
#include <iostream>
#include <andrei_utils/utils.hpp>

using namespace cv;
using namespace rs2;
using namespace std;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

// Convert frame to Mat
Mat frame_to_mat(const frame &f) {
    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    if (f.get_profile().format() == RS2_FORMAT_BGR8) {
        return Mat(Size(w, h), CV_8UC3, (void *) f.get_data(), Mat::AUTO_STEP);
    } else if (f.get_profile().format() == RS2_FORMAT_RGB8) {
        auto r_rgb = Mat(Size(w, h), CV_8UC3, (void *) f.get_data(), Mat::AUTO_STEP);
        Mat r_bgr;
        cvtColor(r_rgb, r_bgr, COLOR_RGB2BGR);
        return r_bgr;
    } else if (f.get_profile().format() == RS2_FORMAT_Z16) {
        return Mat(Size(w, h), CV_16UC1, (void *) f.get_data(), Mat::AUTO_STEP);
    } else if (f.get_profile().format() == RS2_FORMAT_Y8) {
        return Mat(Size(w, h), CV_8UC1, (void *) f.get_data(), Mat::AUTO_STEP);
    } else if (f.get_profile().format() == RS2_FORMAT_DISPARITY32) {
        return Mat(Size(w, h), CV_32FC1, (void *) f.get_data(), Mat::AUTO_STEP);
    }

    throw runtime_error("Frame format is not supported yet!");
}

#pragma clang diagnostic pop

// Converts depth frame to a matrix of doubles with distances in meters
Mat depth_frame_to_meters(const depth_frame &f) {
    Mat dm = frame_to_mat(f);
    dm.convertTo(dm, CV_64F);
    dm = dm * f.get_units();
    return dm;
}

void getRealsenseDepthPointFromImagePixel(void *depthData, DepthDataType dataType, float x, float y,
                                          float (&point)[3], int windowSize, bool forceWindowUsage) {
    if (depthData == nullptr) {
        return;
    }
    rs2_intrinsics intrinsics;
    int width, height;
    std::function<float(int, int)> getDepth;
    switch (dataType) {
        case DepthDataType::DEPTH_FRAME: {
            auto depthFrame = *(depth_frame *) depthData;
            height = depthFrame.get_height();
            width = depthFrame.get_width();
            intrinsics = depthFrame.get_profile().as<video_stream_profile>().get_intrinsics();
            getDepth = [depthFrame](int _x, int _y) { return depthFrame.get_distance(_x, _y); };
            // depth = depthFrame.get_distance((int) x, (int) y);
            break;
        }
        case DepthDataType::DEPTH_MATRIX_INTRINSICS: {
            auto depthTuple = *(tuple<Mat *, rs2_intrinsics *> *) depthData;
            auto depthMat = get<0>(depthTuple);
            height = depthMat->rows;
            width = depthMat->cols;
            // depth = depthMat->at<double>((int) y, (int) x);
            getDepth = [depthMat](int _x, int _y) { return depthMat->at<double>(_y, _x); };
            intrinsics = *get<1>(depthTuple);
            break;
        }
        default: {
            cerr << "Expected different depth data type: " << dataType << endl;
            assert(false);
        }
    }

    int int_x = int(x), int_y = int(y);
    float position[2] = {x, y};
    // rs2_deproject_pixel_to_point(point, &intrinsics, position, getDepth(int_x, int_y));
    // return;

    float depth = getDepth(int_x, int_y), avgDepth;
    auto isDepthInvalid = [](float x) { return lessEqual(x, 0.1f) || lessEqual(10.0f, x); };
    if (forceWindowUsage || isDepthInvalid(depth)) {
        int nrPoints = 0;
        for (int i = max(-int_x, -windowSize); i < min(windowSize, height - int_x); i++) {
            for (int j = max(-int_y, -windowSize); j < min(windowSize, width - int_y); j++) {
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


void getImagePixelFromRealsenseDepthPoint(rs2_intrinsics *intrinsics, float point[3], float (&pixel)[2]) {
    // project pixel from point in 3D
    rs2_project_point_to_pixel(pixel, intrinsics, point);
}