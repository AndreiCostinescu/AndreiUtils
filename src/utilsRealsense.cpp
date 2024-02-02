//
// Created by Andrei on 13.11.20.
//

#include <AndreiUtils/utilsRealsense.h>
#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsString.h>
#include <cassert>
#include <cstring>
#include <librealsense2/rsutil.h>

#ifdef WITH_OPENMP
#include <AndreiUtils/utilsOpenMP.hpp>
#endif

using namespace rs2;
using namespace std;

int convertFrameTypeToDataType(size_t const elementSize, int const nrChannels) {
    int dataType = (nrChannels - 1) << 3;
    if (elementSize == 1) {
        dataType += 0;
    } else if (elementSize == 2) {
        dataType += 2;
    } else if (elementSize == 4) {
        dataType += 5;
    }
    return dataType;
}

void AndreiUtils::frameToBytes(rs2::frame const &f, uint8_t **data, int &h, int &w, int &c, StandardTypes &dataType,
                               bool copyData) {
    // This could be replaced by making data a reference pointer (uint8_t *&data), but casting would become problematic
    if (data == nullptr || (copyData && *data == nullptr)) {
        throw runtime_error("Data container of realsense-frame-data is nullptr...");
    }

    auto vf = f.as<video_frame>();
    w = vf.get_width();
    h = vf.get_height();

    size_t elementSize;
    if (f.get_profile().format() == RS2_FORMAT_BGR8 || f.get_profile().format() == RS2_FORMAT_RGB8) {
        elementSize = sizeof(uint8_t);
        dataType = StandardTypes::TYPE_UINT_8;
        c = 3;
    } else if (f.get_profile().format() == RS2_FORMAT_Z16) {
        elementSize = sizeof(uint16_t);
        dataType = StandardTypes::TYPE_UINT_16;
        c = 1;
    } else if (f.get_profile().format() == RS2_FORMAT_Y8) {
        elementSize = sizeof(uint8_t);
        dataType = StandardTypes::TYPE_UINT_8;
        c = 1;
    } else if (f.get_profile().format() == RS2_FORMAT_DISPARITY32) {
        elementSize = sizeof(uint32_t);
        dataType = StandardTypes::TYPE_UINT_32;
        c = 1;
    } else {
        throw runtime_error("Frame format is not supported yet!");
    }

    // size_t frameElements = w * h * c;
    if (vf.get_bytes_per_pixel() != c * elementSize) {
        size_t nrBytes = w * h * c * elementSize;
        int dataElements = w * h * vf.get_bytes_per_pixel();
        throw runtime_error("The container data and the frame data do not have the same amount of bytes: " +
                            to_string(dataElements) + " vs. " + to_string(nrBytes));
    }
    if (copyData) {
        size_t nrBytes = w * h * c * elementSize;
        #ifdef WITH_OPENMP
        fastMemCopy(*data, (const uint8_t *) f.get_data(), nrBytes);
        #else
        memcpy(*data, (const uint8_t *) f.get_data(), nrBytes);
        #endif
        if (f.get_profile().format() == RS2_FORMAT_BGR8) {
            // Switch BGR to RGB format
            #ifdef WITH_OPENMP
            fastForLoop<uint8_t>(*data, nrBytes, [](uint8_t *const array, size_t i, size_t increment) {
                swapData(array[i], array[i + 2]);
            }, 3);
            #else
            for (size_t i = 0; i < nrBytes; i += 3) {
                swapData(data[i], data[i + 2]);
            }
            #endif
        }
    } else {
        *data = (uint8_t *) f.get_data();
    }
}

void AndreiUtils::frameToBytes(rs2::frame const &f, uint8_t **data, int &dataType, size_t const dataElements) {
    int h, w, c, elementSize;
    StandardTypes type;
    frameToBytes(f, data, h, w, c, type, true);
    elementSize = getStandardTypeByteAmount(type);
    if (dataElements != h * w * c * elementSize) {
        throw runtime_error("The container data and the frame data do not have the same amount of bytes: " +
                            to_string(dataElements) + " vs. " + to_string(h * w * c * elementSize));
    }
    dataType = convertFrameTypeToDataType(elementSize, c);
}

void AndreiUtils::depthFrameToMeters(rs2::depth_frame const &f, double *&data, size_t const dataElements) {
    auto *tmpData = new uint16_t[dataElements];
    int dataType;
    frameToBytes(f, (uint8_t **) &tmpData, dataType, dataElements * sizeof(uint16_t));
    assert (dataType == convertFrameTypeToDataType(2, 1));
    if (data != nullptr) {
        #ifdef WITH_OPENMP
        fastSrcOp<uint16_t, double>(data, (uint16_t *) tmpData, dataElements,
                                    [&](const uint16_t &x) { return (double) x * f.get_units(); });
        #else
        for (size_t i = 0; i < dataElements; i++) {
            data[i] = double(tmpData[i]) * f.get_units();
        }
        #endif
    }
    delete[] tmpData;
}

void AndreiUtils::depthFrameToMilliMeters(rs2::depth_frame const &f, uint16_t *&data, size_t const dataElements) {
    int dataType;
    frameToBytes(f, (uint8_t **) &data, dataType, dataElements * sizeof(uint16_t));
    assert (dataType == convertFrameTypeToDataType(2, 1));
    if (data != nullptr) {
        #ifdef WITH_OPENMP
        fastSrcOp<uint16_t, uint16_t>(data, data, dataElements, [&](const uint16_t &x) {
            return (uint16_t) ((double) x * f.get_units() * 1000);
        });
        #else
        for (size_t i = 0; i < dataElements; i++) {
            data[i] = uint16_t(double(data[i]) * f.get_units() * 1000);
        }
        #endif
    }
}

void AndreiUtils::fromImagePixelTo3dPoint(rs2_intrinsics const &intrinsics, float const position[2], float depth,
                                          float (&point)[3]) {
    rs2_deproject_pixel_to_point(point, &intrinsics, position, depth);
}

void AndreiUtils::fromImagePixelTo3dPoint(rs2_intrinsics const &intrinsics, float x, float y, float depth,
                                          float (&point)[3]) {
    float position[2];
    position[0] = x;
    position[1] = y;
    fromImagePixelTo3dPoint(intrinsics, position, depth, point);
}

void AndreiUtils::getRealsenseDepthPointFromImagePixel(
        function<float(int, int)> const &getDepth, rs2_intrinsics const &intrinsics, float x, float y,
        float (&point)[3], int windowSize, bool forceWindowUsage, float farthestAllowedDepth) {
    int width = intrinsics.width, height = intrinsics.height;
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
    fromImagePixelTo3dPoint(intrinsics, position, avgDepth, point);
}

void AndreiUtils::getRealsenseDepthPointFromImagePixel(
        function<float(int, int)> const &getDepth, ImageParameters const &size,
        CameraIntrinsicParameters const &intrinsics, float x, float y, float (&point)[3], int windowSize,
        bool forceWindowUsage, float farthestAllowedDepth) {
    return getRealsenseDepthPointFromImagePixel(
            getDepth, convertCameraIntrinsicParametersToRealsenseIntrinsics(size, intrinsics), x, y, point, windowSize,
            forceWindowUsage, farthestAllowedDepth);
}

void AndreiUtils::getImagePixelFromRealsenseDepthPoint(rs2_intrinsics const *intrinsics, float point[3],
                                                       float (&pixel)[2]) {
    // project pixel from point in 3D
    rs2_project_point_to_pixel(pixel, intrinsics, point);
}

void AndreiUtils::getImagePixelFromRealsenseDepthPoint(
        ImageParameters const &size, CameraIntrinsicParameters const &intrinsics, float point[3], float (&pixel)[2]) {
    rs2_intrinsics i = convertCameraIntrinsicParametersToRealsenseIntrinsics(size, intrinsics);
    getImagePixelFromRealsenseDepthPoint(&i, point, pixel);
}

rs2_intrinsics AndreiUtils::convertCameraIntrinsicParametersToRealsenseIntrinsics(
        AndreiUtils::ImageParameters const &size, AndreiUtils::CameraIntrinsicParameters const &intrinsics) {
    rs2_intrinsics i;
    i.width = size.w;
    i.height = size.h;
    i.fx = (float) intrinsics.fx;
    i.fy = (float) intrinsics.fy;
    i.ppx = (float) intrinsics.ppx;
    i.ppy = (float) intrinsics.ppy;
    i.ppy = (float) intrinsics.ppy;
    i.model = convertImageDistortionModelToRealsenseDistortion(intrinsics.distortionModel);
    for (int index = 0; index < intrinsics.nrDistortionCoefficients; index++) {
        i.coeffs[index] = intrinsics.distortionCoefficients[index];
    }
    return i;
}

void AndreiUtils::convertRealsenseIntrinsicsToCameraIntrinsicParameters(
        rs2_intrinsics const &rsIntrinsics, AndreiUtils::ImageParameters &size,
        AndreiUtils::CameraIntrinsicParameters &intrinsics) {
    size.setImageParameters(rsIntrinsics.height, rsIntrinsics.width);
    intrinsics.setImageParameters(rsIntrinsics.fx, rsIntrinsics.fy, rsIntrinsics.ppx, rsIntrinsics.ppy);
    intrinsics.setDistortionParameters(convertRealsenseDistortionToImageDistortionModel(rsIntrinsics.model),
                                       rsIntrinsics.coeffs);
}

AndreiUtils::ImageDistortionModel AndreiUtils::convertRealsenseDistortionToImageDistortionModel(
        rs2_distortion const &distortion) {
    switch (distortion) {
        case RS2_DISTORTION_NONE: {
            return ImageDistortionModel::DISTORTION_NONE;
        }
        case RS2_DISTORTION_MODIFIED_BROWN_CONRADY: {
            return ImageDistortionModel::DISTORTION_MODIFIED_BROWN_CONRADY;
        }
        case RS2_DISTORTION_INVERSE_BROWN_CONRADY: {
            return ImageDistortionModel::DISTORTION_INVERSE_BROWN_CONRADY;
        }
        case RS2_DISTORTION_FTHETA: {
            return ImageDistortionModel::DISTORTION_FTHETA;
        }
        case RS2_DISTORTION_BROWN_CONRADY: {
            return ImageDistortionModel::DISTORTION_BROWN_CONRADY;
        }
        case RS2_DISTORTION_KANNALA_BRANDT4: {
            return ImageDistortionModel::DISTORTION_KANNALA_BRANDT4;
        }
        case RS2_DISTORTION_COUNT: {
            throw runtime_error("RS2_DISTORTION_COUNT is not a valid distortion model");
        }
        default: {
            throw runtime_error("Unknown rs2_distortion type: " + to_string(distortion));
        }
    }
}

rs2_distortion AndreiUtils::convertImageDistortionModelToRealsenseDistortion(ImageDistortionModel const &distortion) {
    switch (distortion) {
        case ImageDistortionModel::DISTORTION_NONE: {
            return RS2_DISTORTION_NONE;
        }
        case ImageDistortionModel::DISTORTION_MODIFIED_BROWN_CONRADY: {
            return RS2_DISTORTION_MODIFIED_BROWN_CONRADY;
        }
        case ImageDistortionModel::DISTORTION_INVERSE_BROWN_CONRADY: {
            return RS2_DISTORTION_INVERSE_BROWN_CONRADY;
        }
        case ImageDistortionModel::DISTORTION_FTHETA: {
            return RS2_DISTORTION_FTHETA;
        }
        case ImageDistortionModel::DISTORTION_BROWN_CONRADY: {
            return RS2_DISTORTION_BROWN_CONRADY;
        }
        case ImageDistortionModel::DISTORTION_KANNALA_BRANDT4: {
            return RS2_DISTORTION_KANNALA_BRANDT4;
        }
        default: {
            throw runtime_error("Unknown or incompatible distortion type: " + to_string(distortion));
        }
    }
}

rs2_option AndreiUtils::getRealsenseOptionFromString(std::string const &option) {
    auto optionLower = AndreiUtils::toLowerString(option);
    if (optionLower == "backlight compensation") {
        return RS2_OPTION_BACKLIGHT_COMPENSATION;
    } else if (optionLower == "brightness") {
        return RS2_OPTION_BRIGHTNESS;
    } else if (optionLower == "contrast") {
        return RS2_OPTION_CONTRAST;
    } else if (optionLower == "exposure") {
        return RS2_OPTION_EXPOSURE;
    } else if (optionLower == "gain") {
        return RS2_OPTION_GAIN;
    } else if (optionLower == "gamma") {
        return RS2_OPTION_GAMMA;
    } else if (optionLower == "hue") {
        return RS2_OPTION_HUE;
    } else if (optionLower == "saturation") {
        return RS2_OPTION_SATURATION;
    } else if (optionLower == "white balance") {
        return RS2_OPTION_WHITE_BALANCE;
    } else if (optionLower == "enable auto exposure") {
        return RS2_OPTION_ENABLE_AUTO_EXPOSURE;
    } else if (optionLower == "enable auto white balance") {
        return RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE;
    } else if (optionLower == "visual preset") {
        return RS2_OPTION_VISUAL_PRESET;
    } else if (optionLower == "laser power") {
        return RS2_OPTION_LASER_POWER;
    } else if (optionLower == "accuracy") {
        return RS2_OPTION_ACCURACY;
    } else if (optionLower == "motion range") {
        return RS2_OPTION_MOTION_RANGE;
    } else if (optionLower == "filter option") {
        return RS2_OPTION_FILTER_OPTION;
    } else if (optionLower == "confidence threshold") {
        return RS2_OPTION_CONFIDENCE_THRESHOLD;
    } else if (optionLower == "emitter enabled") {
        return RS2_OPTION_EMITTER_ENABLED;
    } else if (optionLower == "frames queue size") {
        return RS2_OPTION_FRAMES_QUEUE_SIZE;
    } else if (optionLower == "total frame drops") {
        return RS2_OPTION_TOTAL_FRAME_DROPS;
    } else if (optionLower == "fisheye auto exposure mode") {
        return RS2_OPTION_AUTO_EXPOSURE_MODE;
    } else if (optionLower == "power line frequency") {
        return RS2_OPTION_POWER_LINE_FREQUENCY;
    } else if (optionLower == "asic temperature") {
        return RS2_OPTION_ASIC_TEMPERATURE;
    } else if (optionLower == "error polling enabled") {
        return RS2_OPTION_ERROR_POLLING_ENABLED;
    } else if (optionLower == "projector temperature") {
        return RS2_OPTION_PROJECTOR_TEMPERATURE;
    } else if (optionLower == "output trigger enabled") {
        return RS2_OPTION_OUTPUT_TRIGGER_ENABLED;
    } else if (optionLower == "motion module temperature") {
        return RS2_OPTION_MOTION_MODULE_TEMPERATURE;
    } else if (optionLower == "enable motion correction") {
        return RS2_OPTION_ENABLE_MOTION_CORRECTION;
    } else if (optionLower == "auto exposure priority") {
        return RS2_OPTION_AUTO_EXPOSURE_PRIORITY;
    } else if (optionLower == "color scheme") {
        return RS2_OPTION_COLOR_SCHEME;
    } else if (optionLower == "histogram equalization enabled") {
        return RS2_OPTION_HISTOGRAM_EQUALIZATION_ENABLED;
    } else if (optionLower == "min distance") {
        return RS2_OPTION_MIN_DISTANCE;
    } else if (optionLower == "max distance") {
        return RS2_OPTION_MAX_DISTANCE;
    } else if (optionLower == "texture source") {
        return RS2_OPTION_TEXTURE_SOURCE;
    } else if (optionLower == "filter magnitude") {
        return RS2_OPTION_FILTER_MAGNITUDE;
    } else if (optionLower == "filter smooth alpha") {
        return RS2_OPTION_FILTER_SMOOTH_ALPHA;
    } else if (optionLower == "filter smooth delta") {
        return RS2_OPTION_FILTER_SMOOTH_DELTA;
    } else if (optionLower == "holes fill") {
        return RS2_OPTION_HOLES_FILL;
    } else if (optionLower == "stereo baseline") {
        return RS2_OPTION_STEREO_BASELINE;
    } else if (optionLower == "auto exposure converge step") {
        return RS2_OPTION_AUTO_EXPOSURE_CONVERGE_STEP;
    } else if (optionLower == "inter cam sync mode") {
        return RS2_OPTION_INTER_CAM_SYNC_MODE;
    } else if (optionLower == "stream filter") {
        return RS2_OPTION_STREAM_FILTER;
    } else if (optionLower == "stream format filter") {
        return RS2_OPTION_STREAM_FORMAT_FILTER;
    } else if (optionLower == "stream index filter") {
        return RS2_OPTION_STREAM_INDEX_FILTER;
    } else if (optionLower == "emitter on off") {
        return RS2_OPTION_EMITTER_ON_OFF;
    } else if (optionLower == "zero order point x") {
        return RS2_OPTION_ZERO_ORDER_POINT_X;
    } else if (optionLower == "zero order point y") {
        return RS2_OPTION_ZERO_ORDER_POINT_Y;
    } else if (optionLower == "ldd temperature") {
        return RS2_OPTION_LLD_TEMPERATURE;
    } else if (optionLower == "mc temperature") {
        return RS2_OPTION_MC_TEMPERATURE;
    } else if (optionLower == "ma temperature") {
        return RS2_OPTION_MA_TEMPERATURE;
    } else if (optionLower == "hardware preset") {
        return RS2_OPTION_HARDWARE_PRESET;
    } else if (optionLower == "global time enabled") {
        return RS2_OPTION_GLOBAL_TIME_ENABLED;
    } else if (optionLower == "apd temperature") {
        return RS2_OPTION_APD_TEMPERATURE;
    } else if (optionLower == "enable mapping") {
        return RS2_OPTION_ENABLE_MAPPING;
    } else if (optionLower == "enable relocalization") {
        return RS2_OPTION_ENABLE_RELOCALIZATION;
    } else if (optionLower == "enable pose jumping") {
        return RS2_OPTION_ENABLE_POSE_JUMPING;
    } else if (optionLower == "enable dynamic calibration") {
        return RS2_OPTION_ENABLE_DYNAMIC_CALIBRATION;
    } else if (optionLower == "depth offset") {
        return RS2_OPTION_DEPTH_OFFSET;
    } else if (optionLower == "led power") {
        return RS2_OPTION_LED_POWER;
    } else if (optionLower == "zero order enabled") {
        return RS2_OPTION_ZERO_ORDER_ENABLED;
    } else if (optionLower == "enable map preservation") {
        return RS2_OPTION_ENABLE_MAP_PRESERVATION;
    } else if (optionLower == "freefall detection enabled") {
        return RS2_OPTION_FREEFALL_DETECTION_ENABLED;
    } else if (optionLower == "receiver gain") {
        return RS2_OPTION_AVALANCHE_PHOTO_DIODE;
    } else if (optionLower == "post processing sharpening") {
        return RS2_OPTION_POST_PROCESSING_SHARPENING;
    } else if (optionLower == "pre processing sharpening") {
        return RS2_OPTION_PRE_PROCESSING_SHARPENING;
    } else if (optionLower == "noise filtering") {
        return RS2_OPTION_NOISE_FILTERING;
    } else if (optionLower == "invalidation bypass") {
        return RS2_OPTION_INVALIDATION_BYPASS;
    } else if (optionLower == "ambient light") {
        return RS2_OPTION_AMBIENT_LIGHT;
    } else if (optionLower == "digital gain") {
        return RS2_OPTION_DIGITAL_GAIN;
    } else if (optionLower == "sensor mode") {
        return RS2_OPTION_SENSOR_MODE;
    } else if (optionLower == "emitter always on") {
        return RS2_OPTION_EMITTER_ALWAYS_ON;
    } else if (optionLower == "thermal compensation") {
        return RS2_OPTION_THERMAL_COMPENSATION;
    } else if (optionLower == "trigger camera accuracy health") {
        return RS2_OPTION_TRIGGER_CAMERA_ACCURACY_HEALTH;
    } else if (optionLower == "reset camera accuracy health") {
        return RS2_OPTION_RESET_CAMERA_ACCURACY_HEALTH;
    } else if (optionLower == "host performance") {
        return RS2_OPTION_HOST_PERFORMANCE;
    } else if (optionLower == "hdr enabled") {
        return RS2_OPTION_HDR_ENABLED;
    } else if (optionLower == "sequence name") {
        return RS2_OPTION_SEQUENCE_NAME;
    } else if (optionLower == "sequence size") {
        return RS2_OPTION_SEQUENCE_SIZE;
    } else if (optionLower == "sequence id") {
        return RS2_OPTION_SEQUENCE_ID;
    } else if (optionLower == "humidity temperature") {
        return RS2_OPTION_HUMIDITY_TEMPERATURE;
    } else if (optionLower == "enable max usable range") {
        return RS2_OPTION_ENABLE_MAX_USABLE_RANGE;
    } else if (optionLower == "alternate ir") {
        return RS2_OPTION_ALTERNATE_IR;
    } else if (optionLower == "noise estimation") {
        return RS2_OPTION_NOISE_ESTIMATION;
    } else if (optionLower == "enable ir reflectivity") {
        return RS2_OPTION_ENABLE_IR_REFLECTIVITY;
    } else if (optionLower == "auto exposure limit") {
        return RS2_OPTION_AUTO_EXPOSURE_LIMIT;
    } else if (optionLower == "auto gain limit") {
        return RS2_OPTION_AUTO_GAIN_LIMIT;
    } else if (optionLower == "auto rx sensitivity") {
        return RS2_OPTION_AUTO_RX_SENSITIVITY;
    } else if (optionLower == "transmitter frequency") {
        return RS2_OPTION_TRANSMITTER_FREQUENCY;
    } else if (optionLower == "vertical binning") {
        return RS2_OPTION_VERTICAL_BINNING;
    } else if (optionLower == "receiver sensitivity") {
        return RS2_OPTION_RECEIVER_SENSITIVITY;
    } else if (optionLower == "auto exposure limit toggle") {
        return RS2_OPTION_AUTO_EXPOSURE_LIMIT_TOGGLE;
    } else if (optionLower == "auto gain limit toggle") {
        return RS2_OPTION_AUTO_GAIN_LIMIT_TOGGLE;
    } else if (optionLower == "emitter frequency") {
        return RS2_OPTION_EMITTER_FREQUENCY;
    } else if (optionLower == "auto exposure mode") {
        return RS2_OPTION_DEPTH_AUTO_EXPOSURE_MODE;
    } else if (optionLower == "count") {
        return RS2_OPTION_COUNT;
    } else {
        throw std::runtime_error("Unknown realsense option: " + option);
    }
}
