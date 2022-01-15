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

AndreiUtils::ImageDistortionModel AndreiUtils::convertStringToImageDistortionModel(const std::string &distortion) {
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
