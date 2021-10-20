//
// Created by Andrei on 13.11.20.
//

#include <AndreiUtils/utilsRealsense.h>
#include <AndreiUtils/utils.hpp>
#include <librealsense2/rsutil.h>
#include <iostream>

using namespace rs2;
using namespace std;

void AndreiUtils::frameToBytes(const rs2::frame &f, uint8_t *&data, int &dataType, const int dataElements) {

}

void AndreiUtils::depthFrameToMeters(const rs2::depth_frame &f, double *data, const int dataElements) {

}

void AndreiUtils::getRealsenseDepthPointFromImagePixel(function<float(int, int)> &getDepth, rs2_intrinsics &intrinsics,
                                                       float x, float y, float (&point)[3], int windowSize,
                                                       bool forceWindowUsage) {
    int width = intrinsics.width, height = intrinsics.height;
    int int_x = int(x), int_y = int(y);
    float position[2] = {x, y};
    // rs2_deproject_pixel_to_point(point, &intrinsics, position, getDepth(int_x, int_y));
    // return;

    float depth = getDepth(int_x, int_y), avgDepth;
    auto isDepthInvalid = [](float x) { return lessEqual(x, 0.1f) || lessEqual(10.0f, x); };
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