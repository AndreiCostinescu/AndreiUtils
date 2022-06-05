//
// Created by Andrei on 13.11.20.
//

#include <AndreiUtils/utilsOpenCVRealsense.h>
#include <AndreiUtils/utilsRealsense.h>

using namespace AndreiUtils;
using namespace cv;
using namespace rs2;
using namespace std;

// Wrap frame as Mat
Mat AndreiUtils::frame_to_mat(const frame &f) {
    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    int matType;
    if (f.get_profile().format() == RS2_FORMAT_BGR8 || f.get_profile().format() == RS2_FORMAT_RGB8) {
        matType = CV_8UC3;
    } else if (f.get_profile().format() == RS2_FORMAT_Z16) {
        matType = CV_16UC1;
    } else if (f.get_profile().format() == RS2_FORMAT_Y8) {
        matType = CV_8UC1;
    } else if (f.get_profile().format() == RS2_FORMAT_DISPARITY32) {
        matType = CV_32FC1;
    } else {
        throw runtime_error("Frame format is not supported yet!");
    }

    Mat m(Size(w, h), matType, (void *) f.get_data(), Mat::AUTO_STEP);
    if (f.get_profile().format() == RS2_FORMAT_RGB8) {
        Mat r_bgr;
        cvtColor(m, r_bgr, COLOR_RGB2BGR);
        return r_bgr;
    }
    return m;
}

// Convert frame to Mat (allocates new data!)
Mat AndreiUtils::convertFrameToMat(const frame &f) {
    int w, h, c;
    StandardTypes t;

    uchar *dataPtr = nullptr;
    frameToBytes(f, &dataPtr, h, w, c, t, false);
    size_t nrBytes = w * h * c * getStandardTypeByteAmount(t);
    uchar *matData = fastCreateCopy(dataPtr, nrBytes);

    Mat m(Size(w, h), CV_MAKETYPE(convertStandardTypesToOpenCVType(t), c), matData, Mat::AUTO_STEP);

    if (f.get_profile().format() == RS2_FORMAT_RGB8) {
        cvtColor(m, m, COLOR_RGB2BGR);
        assert(m.data == matData);
        /*
        // Switch RGB to BGR format
        fastForLoop<uint8_t>(matData, nrBytes, [](uint8_t *const array, size_t i, size_t increment) {
            swapData(array[i], array[i + 2]);
        }, 3);
        //*/
    }
    return m;
}

// Wraps depth frame as a matrix of doubles with distances in meters
Mat AndreiUtils::depth_frame_to_meters(const depth_frame &f) {
    Mat dm = frame_to_mat(f);
    dm.convertTo(dm, CV_64F, f.get_units());
    return dm;
}

// Converts depth frame to a matrix of doubles with distances in meters (allocates new data!)
Mat AndreiUtils::convertDepthFrameToMeters(const rs2::depth_frame &f) {
    Mat dm = convertFrameToMat(f);
    dm.convertTo(dm, CV_64F, f.get_units());
    return dm;
}
