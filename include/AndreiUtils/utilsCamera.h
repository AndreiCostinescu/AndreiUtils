//
// Created by Andrei on 31-Mar-22.
//

#ifndef ANDREIUTILS_UTILSCAMERA_H
#define ANDREIUTILS_UTILSCAMERA_H

#include <AndreiUtils/classes/camera/CameraIntrinsicParameters.h>
#include <AndreiUtils/classes/camera/ImageParameters.h>
#include <functional>

namespace AndreiUtils {
    void fromImagePixelTo3dPoint(float (&point)[3], AndreiUtils::CameraIntrinsicParameters const &intrinsics,
                                 float const (&pixel)[2], float const &depth);

    void fromImagePixelTo3dPoint(float (&point)[3], AndreiUtils::CameraIntrinsicParameters const &intrinsics,
                                 float const &x, float const &y, float const &depth);

    void fromImagePixelTo3dPoint(double (&point)[3], AndreiUtils::CameraIntrinsicParameters const &intrinsics,
                                 double const (&pixel)[2], double const &depth);

    void fromImagePixelTo3dPoint(double (&point)[3], AndreiUtils::CameraIntrinsicParameters const &intrinsics,
                                 double const &x, double const &y, double const &depth);

    void from3dPointToImagePixel(float (&pixel)[2], AndreiUtils::CameraIntrinsicParameters const &intrinsics,
                                 float const (&point)[3]);

    void from3dPointToImagePixel(double (&pixel)[2], AndreiUtils::CameraIntrinsicParameters const &intrinsics,
                                 double const (&point)[3]);

    void point3dFromPixel(std::function<float(int, int)> const &getDepth, AndreiUtils::ImageParameters const &size,
                          AndreiUtils::CameraIntrinsicParameters const &intrinsics, float x, float y, float (&point)[3],
                          int windowSize = 1, bool forceWindowUsage = false, float farthestAllowedDepth = 10);

    void point3dFromPixel(std::function<double(int, int)> const &getDepth, AndreiUtils::ImageParameters const &size,
                          AndreiUtils::CameraIntrinsicParameters const &intrinsics, double x, double y,
                          double (&point)[3], int windowSize = 1, bool forceWindowUsage = false,
                          double farthestAllowedDepth = 10);
}

#endif //ANDREIUTILS_UTILSCAMERA_H
