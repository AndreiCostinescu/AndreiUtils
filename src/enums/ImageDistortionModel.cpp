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
// Created by Andrei on 20-Oct-21.
//

#include <AndreiUtils/enums/ImageDistortionModel.h>
#include <stdexcept>

using namespace std;

int AndreiUtils::getNumberOfDistortionCoefficients(const ImageDistortionModel &distortion) {
    switch (distortion) {
        case DISTORTION_NONE: {
            return 0;
        }
        case DISTORTION_MODIFIED_BROWN_CONRADY:
        case DISTORTION_INVERSE_BROWN_CONRADY:
        case DISTORTION_FTHETA:
        case DISTORTION_BROWN_CONRADY: {
            return 5;
        }
        case DISTORTION_KANNALA_BRANDT4: {
            return 4;
        }
        default: {
            throw runtime_error("Unknown distortion model: " + to_string(distortion));
        }
    }
}

std::string AndreiUtils::convertImageDistortionModelToString(const ImageDistortionModel &distortion) {
    switch (distortion) {
        case ImageDistortionModel::DISTORTION_NONE: {
            return "None";
        }
        case ImageDistortionModel::DISTORTION_MODIFIED_BROWN_CONRADY: {
            return "Modified Brown Conrady";
        }
        case ImageDistortionModel::DISTORTION_INVERSE_BROWN_CONRADY: {
            return "Inverse Brown Conrady";
        }
        case ImageDistortionModel::DISTORTION_FTHETA: {
            return "Ftheta";
        }
        case ImageDistortionModel::DISTORTION_BROWN_CONRADY: {
            return "Brown Conrady";
        }
        case ImageDistortionModel::DISTORTION_KANNALA_BRANDT4: {
            return "Kannala Brandt4";
        }
        default: {
            throw runtime_error("Unknown ImageDistortionModel " + to_string(distortion));
        }
    }
}

AndreiUtils::ImageDistortionModel AndreiUtils::convertStringToImageDistortionModel(std::string const &distortion) {
    if (distortion == "None") {
        return ImageDistortionModel::DISTORTION_NONE;
    } else if (distortion == "Modified Brown Conrady") {
        return ImageDistortionModel::DISTORTION_MODIFIED_BROWN_CONRADY;
    } else if (distortion == "Inverse Brown Conrady") {
        return ImageDistortionModel::DISTORTION_INVERSE_BROWN_CONRADY;
    } else if (distortion == "Ftheta") {
        return ImageDistortionModel::DISTORTION_FTHETA;
    } else if (distortion == "Brown Conrady") {
        return ImageDistortionModel::DISTORTION_BROWN_CONRADY;
    } else if (distortion == "Kannala Brandt4") {
        return ImageDistortionModel::DISTORTION_KANNALA_BRANDT4;
    } else {
        throw runtime_error("Can not convert " + distortion + " to ImageDistortionModel!");
    }
}
