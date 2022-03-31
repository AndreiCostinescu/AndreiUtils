//
// Created by Andrei on 31-Mar-22.
//

#include <AndreiUtils/utilsCamera.h>
#include <AndreiUtils/utils.hpp>
#include <cfloat>

using namespace AndreiUtils;
using namespace std;

// from librealsense
void AndreiUtils::fromImagePixelTo3dPoint(float (&point)[3], const CameraIntrinsicParameters &intrinsics,
                                          const float (&pixel)[2], const float &depth) {
    // Cannot de-project from a forward-distorted image
    assert(intrinsics.distortionModel != ImageDistortionModel::DISTORTION_MODIFIED_BROWN_CONRADY);

    auto x = (float) ((pixel[0] - intrinsics.ppx) / intrinsics.fx);
    auto y = (float) ((pixel[1] - intrinsics.ppy) / intrinsics.fy);
    if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_INVERSE_BROWN_CONRADY) {
        float r2 = x * x + y * y;
        float f = 1 + intrinsics.distortionCoefficients[0] * r2 + intrinsics.distortionCoefficients[1] * r2 * r2 +
                  intrinsics.distortionCoefficients[4] * r2 * r2 * r2;
        float ux = x * f + 2 * intrinsics.distortionCoefficients[2] * x * y +
                   intrinsics.distortionCoefficients[3] * (r2 + 2 * x * x);
        float uy = y * f + 2 * intrinsics.distortionCoefficients[3] * x * y +
                   intrinsics.distortionCoefficients[2] * (r2 + 2 * y * y);
        x = ux;
        y = uy;
    } else if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_KANNALA_BRANDT4) {
        float rd = sqrt(x * x + y * y);
        if (rd < FLT_EPSILON) {
            rd = FLT_EPSILON;
        }

        float theta = rd;
        float theta2 = rd * rd;
        float f, df;
        // some sort of newton method
        for (int i = 0; i < 4; i++) {
            f = theta * (1 +
                         theta2 * (intrinsics.distortionCoefficients[0] +
                                   theta2 * (intrinsics.distortionCoefficients[1] +
                                             theta2 * (intrinsics.distortionCoefficients[2] +
                                                       theta2 * intrinsics.distortionCoefficients[3])))) - rd;
            if (abs(f) < FLT_EPSILON) {
                break;
            }
            df = 1 + theta2 * (3 * intrinsics.distortionCoefficients[0] +
                               theta2 * (5 * intrinsics.distortionCoefficients[1] +
                                         theta2 * (7 * intrinsics.distortionCoefficients[2] +
                                                   9 * theta2 * intrinsics.distortionCoefficients[3])));
            theta -= f / df;
            theta2 = theta * theta;
        }
        float r = tan(theta);
        x *= r / rd;
        y *= r / rd;
    } else if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_FTHETA) {
        float rd = sqrt(x * x + y * y);
        if (rd < FLT_EPSILON) {
            rd = FLT_EPSILON;
        }
        float r = tan(intrinsics.distortionCoefficients[0] * rd) /
                  atan(2 * tan(intrinsics.distortionCoefficients[0] / 2.0f));
        x *= r / rd;
        y *= r / rd;
    }

    point[0] = depth * x;
    point[1] = depth * y;
    point[2] = depth;
}

// from librealsense
void AndreiUtils::fromImagePixelTo3dPoint(double (&point)[3], const CameraIntrinsicParameters &intrinsics,
                                          const double (&pixel)[2], const double &depth) {
    // Cannot de-project from a forward-distorted image
    assert(intrinsics.distortionModel != ImageDistortionModel::DISTORTION_MODIFIED_BROWN_CONRADY);

    double x = (pixel[0] - intrinsics.ppx) / intrinsics.fx;
    double y = (pixel[1] - intrinsics.ppy) / intrinsics.fy;
    if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_INVERSE_BROWN_CONRADY) {
        double r2 = x * x + y * y;
        double f = 1 + intrinsics.distortionCoefficients[0] * r2 + intrinsics.distortionCoefficients[1] * r2 * r2 +
                   intrinsics.distortionCoefficients[4] * r2 * r2 * r2;
        double ux = x * f + 2 * intrinsics.distortionCoefficients[2] * x * y +
                    intrinsics.distortionCoefficients[3] * (r2 + 2 * x * x);
        double uy = y * f + 2 * intrinsics.distortionCoefficients[3] * x * y +
                    intrinsics.distortionCoefficients[2] * (r2 + 2 * y * y);
        x = ux;
        y = uy;
    } else if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_KANNALA_BRANDT4) {
        double rd = sqrt(x * x + y * y);
        if (rd < DBL_EPSILON) {
            rd = DBL_EPSILON;
        }

        double theta = rd;
        double theta2 = rd * rd;
        double f, df;
        // some sort of newton method
        for (int i = 0; i < 4; i++) {
            f = theta * (1 +
                         theta2 * (intrinsics.distortionCoefficients[0] +
                                   theta2 * (intrinsics.distortionCoefficients[1] +
                                             theta2 * (intrinsics.distortionCoefficients[2] +
                                                       theta2 * intrinsics.distortionCoefficients[3])))) - rd;
            if (abs(f) < DBL_EPSILON) {
                break;
            }
            df = 1 + theta2 * (3 * intrinsics.distortionCoefficients[0] +
                               theta2 * (5 * intrinsics.distortionCoefficients[1] +
                                         theta2 * (7 * intrinsics.distortionCoefficients[2] +
                                                   9 * theta2 * intrinsics.distortionCoefficients[3])));
            theta -= f / df;
            theta2 = theta * theta;
        }
        double r = tan(theta);
        x *= r / rd;
        y *= r / rd;
    } else if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_FTHETA) {
        double rd = sqrt(x * x + y * y);
        if (rd < DBL_EPSILON) {
            rd = DBL_EPSILON;
        }
        double r = tan(intrinsics.distortionCoefficients[0] * rd) /
                   atan(2 * tan(intrinsics.distortionCoefficients[0] / 2.0));
        x *= r / rd;
        y *= r / rd;
    }

    point[0] = (float) depth * x;
    point[1] = (float) depth * y;
    point[2] = depth;
}

void AndreiUtils::fromImagePixelTo3dPoint(float (&point)[3], const CameraIntrinsicParameters &intrinsics,
                                          const float &x, const float &y, const float &depth) {
    float pixel[2] = {x, y};
    fromImagePixelTo3dPoint(point, intrinsics, pixel, depth);
}

void AndreiUtils::fromImagePixelTo3dPoint(double (&point)[3], const CameraIntrinsicParameters &intrinsics,
                                          const double &x, const double &y, const double &depth) {
    double pixel[2] = {x, y};
    fromImagePixelTo3dPoint(point, intrinsics, pixel, depth);
}

// from librealsense
void AndreiUtils::from3dPointToImagePixel(float (&pixel)[2], const CameraIntrinsicParameters &intrinsics,
                                          const float (&point)[3]) {
    float x = point[0] / point[2], y = point[1] / point[2];

    if ((intrinsics.distortionModel == ImageDistortionModel::DISTORTION_MODIFIED_BROWN_CONRADY) ||
        (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_INVERSE_BROWN_CONRADY)) {
        float r2 = x * x + y * y;
        float f = 1 + intrinsics.distortionCoefficients[0] * r2 + intrinsics.distortionCoefficients[1] * r2 * r2 +
                  intrinsics.distortionCoefficients[4] * r2 * r2 * r2;
        x *= f;
        y *= f;
        float dx = x + 2 * intrinsics.distortionCoefficients[2] * x * y +
                   intrinsics.distortionCoefficients[3] * (r2 + 2 * x * x);
        float dy = y + 2 * intrinsics.distortionCoefficients[3] * x * y +
                   intrinsics.distortionCoefficients[2] * (r2 + 2 * y * y);
        x = dx;
        y = dy;
    } else if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_BROWN_CONRADY) {
        float x2 = x * x, y2 = y * y, xy = x * y, r2 = x2 + y2;
        float f = 1 + intrinsics.distortionCoefficients[0] * r2 + intrinsics.distortionCoefficients[1] * r2 * r2 +
                  intrinsics.distortionCoefficients[4] * r2 * r2 * r2;
        float dx = x * f + 2 * intrinsics.distortionCoefficients[2] * xy +
                   intrinsics.distortionCoefficients[3] * (r2 + 2 * x2);
        float dy = y * f + 2 * intrinsics.distortionCoefficients[3] * xy +
                   intrinsics.distortionCoefficients[2] * (r2 + 2 * y2);
        x = dx;
        y = dy;
    } else if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_FTHETA) {
        float r = sqrt(x * x + y * y);
        if (r < FLT_EPSILON) {
            r = FLT_EPSILON;
        }
        auto rd = (float) (1.0f / intrinsics.distortionCoefficients[0] *
                           atan(2 * r * tan(intrinsics.distortionCoefficients[0] / 2.0f)));
        x *= rd / r;
        y *= rd / r;
    } else if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_KANNALA_BRANDT4) {
        float r = sqrt(x * x + y * y);
        if (r < FLT_EPSILON) {
            r = FLT_EPSILON;
        }
        float theta = atan(r);
        float theta2 = theta * theta;
        float series = 1 + theta2 * (intrinsics.distortionCoefficients[0] +
                                     theta2 * (intrinsics.distortionCoefficients[1] +
                                               theta2 * (intrinsics.distortionCoefficients[2] +
                                                         theta2 * intrinsics.distortionCoefficients[3])));
        float rd = theta * series;
        x *= rd / r;
        y *= rd / r;
    }

    pixel[0] = (float) (x * intrinsics.fx + intrinsics.ppx);
    pixel[1] = (float) (y * intrinsics.fy + intrinsics.ppy);
}

// from librealsense
void AndreiUtils::from3dPointToImagePixel(double (&pixel)[2], const CameraIntrinsicParameters &intrinsics,
                                          const double (&point)[3]) {
    double x = point[0] / point[2], y = point[1] / point[2];

    if ((intrinsics.distortionModel == ImageDistortionModel::DISTORTION_MODIFIED_BROWN_CONRADY) ||
        (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_INVERSE_BROWN_CONRADY)) {
        double r2 = x * x + y * y;
        double f = 1 + intrinsics.distortionCoefficients[0] * r2 + intrinsics.distortionCoefficients[1] * r2 * r2 +
                   intrinsics.distortionCoefficients[4] * r2 * r2 * r2;
        x *= f;
        y *= f;
        double dx = x + 2 * intrinsics.distortionCoefficients[2] * x * y +
                    intrinsics.distortionCoefficients[3] * (r2 + 2 * x * x);
        double dy = y + 2 * intrinsics.distortionCoefficients[3] * x * y +
                    intrinsics.distortionCoefficients[2] * (r2 + 2 * y * y);
        x = dx;
        y = dy;
    } else if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_BROWN_CONRADY) {
        double x2 = x * x, y2 = y * y, xy = x * y, r2 = x2 + y2;
        double f = 1 + intrinsics.distortionCoefficients[0] * r2 + intrinsics.distortionCoefficients[1] * r2 * r2 +
                   intrinsics.distortionCoefficients[4] * r2 * r2 * r2;
        double dx = x * f + 2 * intrinsics.distortionCoefficients[2] * xy +
                    intrinsics.distortionCoefficients[3] * (r2 + 2 * x2);
        double dy = y * f + 2 * intrinsics.distortionCoefficients[3] * xy +
                    intrinsics.distortionCoefficients[2] * (r2 + 2 * y2);
        x = dx;
        y = dy;
    } else if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_FTHETA) {
        double r = sqrt(x * x + y * y);
        if (r < DBL_EPSILON) {
            r = DBL_EPSILON;
        }
        auto rd = (double) (1.0 / intrinsics.distortionCoefficients[0] *
                            atan(2 * r * tan(intrinsics.distortionCoefficients[0] / 2.0)));
        x *= rd / r;
        y *= rd / r;
    } else if (intrinsics.distortionModel == ImageDistortionModel::DISTORTION_KANNALA_BRANDT4) {
        double r = sqrt(x * x + y * y);
        if (r < DBL_EPSILON) {
            r = DBL_EPSILON;
        }
        double theta = atan(r);
        double theta2 = theta * theta;
        double series = 1 + theta2 * (intrinsics.distortionCoefficients[0] +
                                      theta2 * (intrinsics.distortionCoefficients[1] +
                                                theta2 * (intrinsics.distortionCoefficients[2] +
                                                          theta2 * intrinsics.distortionCoefficients[3])));
        double rd = theta * series;
        x *= rd / r;
        y *= rd / r;
    }

    pixel[0] = x * intrinsics.fx + intrinsics.ppx;
    pixel[1] = y * intrinsics.fy + intrinsics.ppy;
}

void AndreiUtils::point3dFromPixel(
        const std::function<float(int, int)> &getDepth, const ImageParameters &size,
        const CameraIntrinsicParameters &intrinsics, float x, float y, float (&point)[3], int windowSize,
        bool forceWindowUsage, float farthestAllowedDepth) {
    int width = size.w, height = size.h;
    int int_x = int(x), int_y = int(y);
    float position[2] = {x, y};
    // rs2_deproject_pixel_to_point(point, &intrinsics, position, getDepth(int_x, int_y));
    // return;

    float depth = getDepth(int_x, int_y), avgDepth = 0;
    auto isDepthInvalid = [farthestAllowedDepth](float x) {
        return isnan(x) || lessEqual(x, 0.1f) || lessEqual(farthestAllowedDepth, x);
    };
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
    fromImagePixelTo3dPoint(point, intrinsics, position, avgDepth);
}

void AndreiUtils::point3dFromPixel(
        const std::function<double(int, int)> &getDepth, const ImageParameters &size,
        const CameraIntrinsicParameters &intrinsics, double x, double y, double (&point)[3], int windowSize,
        bool forceWindowUsage, double farthestAllowedDepth) {
    int width = size.w, height = size.h;
    int int_x = int(x), int_y = int(y);
    double position[2] = {x, y};
    // rs2_deproject_pixel_to_point(point, &intrinsics, position, getDepth(int_x, int_y));
    // return;

    double depth = getDepth(int_x, int_y), avgDepth = 0;
    auto isDepthInvalid = [farthestAllowedDepth](double x) {
        return isnan(x) || lessEqual(x, 0.1) || lessEqual(farthestAllowedDepth, x);
    };
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
    fromImagePixelTo3dPoint(point, intrinsics, position, avgDepth);
}
