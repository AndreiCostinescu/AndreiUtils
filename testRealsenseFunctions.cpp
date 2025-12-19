//
// Created by Andrei on 16.08.23.
//

#include <AndreiUtils/utilsRealsense.h>
#include <iostream>

using namespace AndreiUtils;
using namespace rs2;
using namespace std;

void realsenseDistortionString() {
    cout << rs2_distortion_to_string(RS2_DISTORTION_NONE) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_MODIFIED_BROWN_CONRADY) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_INVERSE_BROWN_CONRADY) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_FTHETA) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_BROWN_CONRADY) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_KANNALA_BRANDT4) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_COUNT) << endl;
}

void testRealsenseOptionsToString() {
    cout << rs2_option_to_string(RS2_OPTION_BACKLIGHT_COMPENSATION) << endl;
    cout << rs2_option_to_string(RS2_OPTION_BRIGHTNESS) << endl;
    cout << rs2_option_to_string(RS2_OPTION_CONTRAST) << endl;
    cout << rs2_option_to_string(RS2_OPTION_EXPOSURE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_GAIN) << endl;
    cout << rs2_option_to_string(RS2_OPTION_GAMMA) << endl;
    cout << rs2_option_to_string(RS2_OPTION_HUE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_SATURATION) << endl;
    cout << rs2_option_to_string(RS2_OPTION_WHITE_BALANCE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ENABLE_AUTO_EXPOSURE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_VISUAL_PRESET) << endl;
    cout << rs2_option_to_string(RS2_OPTION_LASER_POWER) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ACCURACY) << endl;
    cout << rs2_option_to_string(RS2_OPTION_MOTION_RANGE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_FILTER_OPTION) << endl;
    cout << rs2_option_to_string(RS2_OPTION_CONFIDENCE_THRESHOLD) << endl;
    cout << rs2_option_to_string(RS2_OPTION_EMITTER_ENABLED) << endl;
    cout << rs2_option_to_string(RS2_OPTION_FRAMES_QUEUE_SIZE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_TOTAL_FRAME_DROPS) << endl;
    cout << rs2_option_to_string(RS2_OPTION_AUTO_EXPOSURE_MODE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_POWER_LINE_FREQUENCY) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ASIC_TEMPERATURE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ERROR_POLLING_ENABLED) << endl;
    cout << rs2_option_to_string(RS2_OPTION_PROJECTOR_TEMPERATURE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_OUTPUT_TRIGGER_ENABLED) << endl;
    cout << rs2_option_to_string(RS2_OPTION_MOTION_MODULE_TEMPERATURE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ENABLE_MOTION_CORRECTION) << endl;
    cout << rs2_option_to_string(RS2_OPTION_AUTO_EXPOSURE_PRIORITY) << endl;
    cout << rs2_option_to_string(RS2_OPTION_COLOR_SCHEME) << endl;
    cout << rs2_option_to_string(RS2_OPTION_HISTOGRAM_EQUALIZATION_ENABLED) << endl;
    cout << rs2_option_to_string(RS2_OPTION_MIN_DISTANCE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_MAX_DISTANCE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_TEXTURE_SOURCE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_FILTER_MAGNITUDE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_FILTER_SMOOTH_ALPHA) << endl;
    cout << rs2_option_to_string(RS2_OPTION_FILTER_SMOOTH_DELTA) << endl;
    cout << rs2_option_to_string(RS2_OPTION_HOLES_FILL) << endl;
    cout << rs2_option_to_string(RS2_OPTION_STEREO_BASELINE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_AUTO_EXPOSURE_CONVERGE_STEP) << endl;
    cout << rs2_option_to_string(RS2_OPTION_INTER_CAM_SYNC_MODE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_STREAM_FILTER) << endl;
    cout << rs2_option_to_string(RS2_OPTION_STREAM_FORMAT_FILTER) << endl;
    cout << rs2_option_to_string(RS2_OPTION_STREAM_INDEX_FILTER) << endl;
    cout << rs2_option_to_string(RS2_OPTION_EMITTER_ON_OFF) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ZERO_ORDER_POINT_X) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ZERO_ORDER_POINT_Y) << endl;
    cout << rs2_option_to_string(RS2_OPTION_LLD_TEMPERATURE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_MC_TEMPERATURE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_MA_TEMPERATURE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_HARDWARE_PRESET) << endl;
    cout << rs2_option_to_string(RS2_OPTION_GLOBAL_TIME_ENABLED) << endl;
    cout << rs2_option_to_string(RS2_OPTION_APD_TEMPERATURE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ENABLE_MAPPING) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ENABLE_RELOCALIZATION) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ENABLE_POSE_JUMPING) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ENABLE_DYNAMIC_CALIBRATION) << endl;
    cout << rs2_option_to_string(RS2_OPTION_DEPTH_OFFSET) << endl;
    cout << rs2_option_to_string(RS2_OPTION_LED_POWER) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ZERO_ORDER_ENABLED) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ENABLE_MAP_PRESERVATION) << endl;
    cout << rs2_option_to_string(RS2_OPTION_FREEFALL_DETECTION_ENABLED) << endl;
    cout << rs2_option_to_string(RS2_OPTION_AVALANCHE_PHOTO_DIODE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_POST_PROCESSING_SHARPENING) << endl;
    cout << rs2_option_to_string(RS2_OPTION_PRE_PROCESSING_SHARPENING) << endl;
    cout << rs2_option_to_string(RS2_OPTION_NOISE_FILTERING) << endl;
    cout << rs2_option_to_string(RS2_OPTION_INVALIDATION_BYPASS) << endl;
    cout << rs2_option_to_string(RS2_OPTION_AMBIENT_LIGHT) << endl;
    cout << rs2_option_to_string(RS2_OPTION_DIGITAL_GAIN) << endl;
    cout << rs2_option_to_string(RS2_OPTION_SENSOR_MODE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_EMITTER_ALWAYS_ON) << endl;
    cout << rs2_option_to_string(RS2_OPTION_THERMAL_COMPENSATION) << endl;
    cout << rs2_option_to_string(RS2_OPTION_TRIGGER_CAMERA_ACCURACY_HEALTH) << endl;
    cout << rs2_option_to_string(RS2_OPTION_RESET_CAMERA_ACCURACY_HEALTH) << endl;
    cout << rs2_option_to_string(RS2_OPTION_HOST_PERFORMANCE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_HDR_ENABLED) << endl;
    cout << rs2_option_to_string(RS2_OPTION_SEQUENCE_NAME) << endl;
    cout << rs2_option_to_string(RS2_OPTION_SEQUENCE_SIZE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_SEQUENCE_ID) << endl;
    cout << rs2_option_to_string(RS2_OPTION_HUMIDITY_TEMPERATURE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ENABLE_MAX_USABLE_RANGE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ALTERNATE_IR) << endl;
    cout << rs2_option_to_string(RS2_OPTION_NOISE_ESTIMATION) << endl;
    cout << rs2_option_to_string(RS2_OPTION_ENABLE_IR_REFLECTIVITY) << endl;
    cout << rs2_option_to_string(RS2_OPTION_AUTO_EXPOSURE_LIMIT) << endl;
    cout << rs2_option_to_string(RS2_OPTION_AUTO_GAIN_LIMIT) << endl;
    cout << rs2_option_to_string(RS2_OPTION_AUTO_RX_SENSITIVITY) << endl;
    cout << rs2_option_to_string(RS2_OPTION_TRANSMITTER_FREQUENCY) << endl;
    cout << rs2_option_to_string(RS2_OPTION_VERTICAL_BINNING) << endl;
    cout << rs2_option_to_string(RS2_OPTION_RECEIVER_SENSITIVITY) << endl;
    cout << rs2_option_to_string(RS2_OPTION_AUTO_EXPOSURE_LIMIT_TOGGLE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_AUTO_GAIN_LIMIT_TOGGLE) << endl;
    cout << rs2_option_to_string(RS2_OPTION_EMITTER_FREQUENCY) << endl;
    #if RS2_API_VERSION >= 25400
    cout << rs2_option_to_string(RS2_OPTION_DEPTH_AUTO_EXPOSURE_MODE) << endl;
    #endif
    cout << rs2_option_to_string(RS2_OPTION_COUNT) << endl;
}

void testRealsenseOptionStringsToString() {
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_BACKLIGHT_COMPENSATION)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_BRIGHTNESS)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_CONTRAST)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_EXPOSURE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_GAIN)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_GAMMA)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_HUE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_SATURATION)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_WHITE_BALANCE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ENABLE_AUTO_EXPOSURE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_VISUAL_PRESET)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_LASER_POWER)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ACCURACY)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_MOTION_RANGE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_FILTER_OPTION)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_CONFIDENCE_THRESHOLD)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_EMITTER_ENABLED)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_FRAMES_QUEUE_SIZE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_TOTAL_FRAME_DROPS)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_AUTO_EXPOSURE_MODE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_POWER_LINE_FREQUENCY)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ASIC_TEMPERATURE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ERROR_POLLING_ENABLED)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_PROJECTOR_TEMPERATURE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_OUTPUT_TRIGGER_ENABLED)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_MOTION_MODULE_TEMPERATURE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ENABLE_MOTION_CORRECTION)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_AUTO_EXPOSURE_PRIORITY)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_COLOR_SCHEME)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_HISTOGRAM_EQUALIZATION_ENABLED)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_MIN_DISTANCE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_MAX_DISTANCE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_TEXTURE_SOURCE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_FILTER_MAGNITUDE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_FILTER_SMOOTH_ALPHA)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_FILTER_SMOOTH_DELTA)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_HOLES_FILL)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_STEREO_BASELINE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_AUTO_EXPOSURE_CONVERGE_STEP)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_INTER_CAM_SYNC_MODE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_STREAM_FILTER)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_STREAM_FORMAT_FILTER)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_STREAM_INDEX_FILTER)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_EMITTER_ON_OFF)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ZERO_ORDER_POINT_X)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ZERO_ORDER_POINT_Y)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_LLD_TEMPERATURE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_MC_TEMPERATURE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_MA_TEMPERATURE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_HARDWARE_PRESET)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_GLOBAL_TIME_ENABLED)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_APD_TEMPERATURE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ENABLE_MAPPING)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ENABLE_RELOCALIZATION)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ENABLE_POSE_JUMPING)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ENABLE_DYNAMIC_CALIBRATION)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_DEPTH_OFFSET)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_LED_POWER)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ZERO_ORDER_ENABLED)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ENABLE_MAP_PRESERVATION)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_FREEFALL_DETECTION_ENABLED)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_AVALANCHE_PHOTO_DIODE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_POST_PROCESSING_SHARPENING)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_PRE_PROCESSING_SHARPENING)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_NOISE_FILTERING)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_INVALIDATION_BYPASS)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_AMBIENT_LIGHT)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_DIGITAL_GAIN)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_SENSOR_MODE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_EMITTER_ALWAYS_ON)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_THERMAL_COMPENSATION)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_TRIGGER_CAMERA_ACCURACY_HEALTH)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_RESET_CAMERA_ACCURACY_HEALTH)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_HOST_PERFORMANCE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_HDR_ENABLED)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_SEQUENCE_NAME)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_SEQUENCE_SIZE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_SEQUENCE_ID)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_HUMIDITY_TEMPERATURE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ENABLE_MAX_USABLE_RANGE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ALTERNATE_IR)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_NOISE_ESTIMATION)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_ENABLE_IR_REFLECTIVITY)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_AUTO_EXPOSURE_LIMIT)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_AUTO_GAIN_LIMIT)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_AUTO_RX_SENSITIVITY)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_TRANSMITTER_FREQUENCY)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_VERTICAL_BINNING)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_RECEIVER_SENSITIVITY)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_AUTO_EXPOSURE_LIMIT_TOGGLE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_AUTO_GAIN_LIMIT_TOGGLE)) << endl;
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_EMITTER_FREQUENCY)) << endl;
    #if RS2_API_VERSION >= 25400
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_DEPTH_AUTO_EXPOSURE_MODE)) << endl;
    #endif
    cout << getRealsenseOptionFromString(rs2_option_to_string(RS2_OPTION_COUNT)) << endl;
}

int main() {
    cout << "Hello World!" << endl;

    // realsenseDistortionString();
    // testRealsenseOptionsToString();
    testRealsenseOptionStringsToString();

    return 0;
}