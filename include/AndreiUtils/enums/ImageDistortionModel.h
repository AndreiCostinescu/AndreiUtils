// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 19.02.21.
//

#ifndef ANDREIUTILS_ENUMS_IMAGEDISTORTIONMODEL_H
#define ANDREIUTILS_ENUMS_IMAGEDISTORTIONMODEL_H

#include <string>

namespace AndreiUtils {
    enum ImageDistortionModel {
        DISTORTION_NONE, /**< Rectilinear images. No distortion compensation required. */
        DISTORTION_MODIFIED_BROWN_CONRADY, /**< Equivalent to Brown-Conrady distortion, except that tangential
                                              distortion is applied to radially distorted points */
        DISTORTION_INVERSE_BROWN_CONRADY, /**< Equivalent to Brown-Conrady distortion, except undistorts image instead
                                             of distorting it */
        DISTORTION_FTHETA, /**< F-Theta fish-eye distortion model */
        DISTORTION_BROWN_CONRADY, /**< Unmodified Brown-Conrady distortion model */
        DISTORTION_KANNALA_BRANDT4, /**< Four parameter Kannala Brandt distortion model */
    };

    int getNumberOfDistortionCoefficients(const ImageDistortionModel &distortion);

    std::string convertImageDistortionModelToString(const ImageDistortionModel &distortion);

    ImageDistortionModel convertStringToImageDistortionModel(std::string const &distortion);
} // namespace AndreiUtils

#endif // ANDREIUTILS_ENUMS_IMAGEDISTORTIONMODEL_H
