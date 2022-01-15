//
// Created by Andrei on 19.02.21.
//

#ifndef ANDREIUTILS_ENUMS_IMAGEDISTORTIONMODEL_H
#define ANDREIUTILS_ENUMS_IMAGEDISTORTIONMODEL_H

#include <string>

namespace AndreiUtils {
    enum ImageDistortionModel {
        DISTORTION_NONE                  , /**< Rectilinear images. No distortion compensation required. */
        DISTORTION_MODIFIED_BROWN_CONRADY, /**< Equivalent to Brown-Conrady distortion, except that tangential distortion is applied to radially distorted points */
        DISTORTION_INVERSE_BROWN_CONRADY , /**< Equivalent to Brown-Conrady distortion, except undistorts image instead of distorting it */
        DISTORTION_FTHETA                , /**< F-Theta fish-eye distortion model */
        DISTORTION_BROWN_CONRADY         , /**< Unmodified Brown-Conrady distortion model */
        DISTORTION_KANNALA_BRANDT4       , /**< Four parameter Kannala Brandt distortion model */
    };

    int getNumberOfDistortionCoefficients(const ImageDistortionModel &distortion);

    std::string convertImageDistortionModelToString(const ImageDistortionModel &distortion);

    ImageDistortionModel convertStringToImageDistortionModel(const std::string &distortion);
}

#endif //ANDREIUTILS_ENUMS_IMAGEDISTORTIONMODEL_H
