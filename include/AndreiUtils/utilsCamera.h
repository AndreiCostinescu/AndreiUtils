//
// Created by Andrei on 31-Mar-22.
//

#ifndef ANDREIUTILS_UTILSCAMERA_H
#define ANDREIUTILS_UTILSCAMERA_H

#include <AndreiUtils/classes/camera/CameraIntrinsicParameters.h>

namespace AndreiUtils {
    void fromImagePixelTo3dPoint(float (&point)[3], const AndreiUtils::CameraIntrinsicParameters &intrinsics,
                                 const float (&pixel)[2], const float &depth);

    void fromImagePixelTo3dPoint(float (&point)[3], const AndreiUtils::CameraIntrinsicParameters &intrinsics,
                                 const float &x, const float &y, const float &depth);

    void fromImagePixelTo3dPoint(double (&point)[3], const AndreiUtils::CameraIntrinsicParameters &intrinsics,
                                 const double (&pixel)[2], const double &depth);

    void fromImagePixelTo3dPoint(double (&point)[3], const AndreiUtils::CameraIntrinsicParameters &intrinsics,
                                 const double &x, const double &y, const double &depth);

    void from3dPointToImagePixel(float (&pixel)[2], const AndreiUtils::CameraIntrinsicParameters &intrinsics,
                                 const float (&point)[3]);

    void from3dPointToImagePixel(double (&pixel)[2], const AndreiUtils::CameraIntrinsicParameters &intrinsics,
                                 const double (&point)[3]);
}

#endif //ANDREIUTILS_UTILSCAMERA_H
