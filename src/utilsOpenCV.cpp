//
// Created by Andrei on 26.11.20.
//

#include <AndreiUtils/utilsOpenCV.h>
#include <AndreiUtils/utilsString.h>
#include <AndreiUtils/utilsOpenMP.hpp>

using namespace cv;
using namespace std;

size_t AndreiUtils::matByteSize(const Mat &mat) {
    if (mat.isContinuous()) {
        return (mat.dataend - mat.datastart);
    }
    return mat.rows * CV_ELEM_SIZE(mat.type()) * mat.cols;
}

void AndreiUtils::imageRotation(Mat *image, RotationType rotation) {
    switch (rotation) {
        case LEFT_90: {
            cv::rotate(*image, *image, cv::ROTATE_90_COUNTERCLOCKWISE);
            break;
        }
        case LEFT_180: {
            cv::rotate(*image, *image, cv::ROTATE_180);
            break;
        }
        case LEFT_270: {
            cv::rotate(*image, *image, cv::ROTATE_90_CLOCKWISE);
            break;
        }
        default: {
            break;
        }
    }
}

uchar *AndreiUtils::copyMatData(const Mat &mat) {
    auto *dataPtr = new uchar[matByteSize(mat)];
    if (mat.isContinuous()) {
        fastMemCopy(dataPtr, mat.ptr(0), (mat.dataend - mat.datastart));
    } else {
        int rowSize = CV_ELEM_SIZE(mat.type()) * mat.cols;
        for (int r = 0; r < mat.rows; ++r) {
            fastMemCopy(dataPtr + r * rowSize, mat.ptr(r), rowSize);
        }
    }
    return dataPtr;
}

void AndreiUtils::matWriteBinary(ofstream *fs, const Mat &mat) {
    // Header
    int type = mat.type();
    int channels = mat.channels();
    fs->write((char *) &mat.rows, sizeof(int));    // rows
    fs->write((char *) &mat.cols, sizeof(int));    // cols
    fs->write((char *) &type, sizeof(int));        // type
    fs->write((char *) &channels, sizeof(int));    // channels

    // Data
    if (mat.isContinuous()) {
        fs->write(mat.ptr<char>(0), (mat.dataend - mat.datastart));
    } else {
        int rowSize = CV_ELEM_SIZE(type) * mat.cols;
        for (int r = 0; r < mat.rows; ++r) {
            fs->write(mat.ptr<char>(r), rowSize);
        }
    }
}

bool AndreiUtils::matReadBinary(ifstream *fs, Mat *result) {
    // Header
    int rows, cols, type, channels;
    if (fs->peek() == EOF) {
        return false;
    }
    fs->read((char *) &rows, sizeof(int));         // rows
    if (fs->fail() || fs->peek() == EOF) {
        return false;
    }
    fs->read((char *) &cols, sizeof(int));         // cols
    if (fs->fail() || fs->peek() == EOF) {
        return false;
    }
    fs->read((char *) &type, sizeof(int));         // type
    if (fs->fail() || fs->peek() == EOF) {
        return false;
    }
    fs->read((char *) &channels, sizeof(int));     // channels
    if (fs->fail() || fs->peek() == EOF) {
        return false;
    }

    // Data
    Mat mat(rows, cols, type);
    fs->read((char *) mat.data, CV_ELEM_SIZE(type) * rows * cols);

    *result = mat;
    return !fs->fail();
}

void AndreiUtils::displayImage(const Mat &image, const char *title, bool verbose) {
    if (verbose) {
        cout << "Printing " << title << "... " << image << endl;
    }
    if (!image.empty()) {
        imshow(title, image);
    } else {
        assert(image.empty());
        cerr << title << " is empty!" << endl;
    }
}

void AndreiUtils::displayImage(const Mat *const image, const char *title, bool verbose) {
    if (verbose) {
        cout << "Printing " << title << "... " << image << endl;
    }
    if (image != nullptr) {
        displayImage(*image, title, false);
    } else {
        assert(image == nullptr);
        cerr << title << " is null!" << endl;
    }
}

void AndreiUtils::displayImages(const vector<Mat> &images, const vector<string> &titles) {
    assert (images.size() >= titles.size());
    for (int i = 0; i < images.size(); i++) {
        displayImage(images[i], titles[i].c_str());
    }
    cout << "Printed!" << endl;
}

void AndreiUtils::displayImages(const vector<const Mat *> &images, const vector<string> &titles) {
    assert (images.size() >= titles.size());
    for (int i = 0; i < images.size(); i++) {
        displayImage(images[i], titles[i].c_str());
    }
    cout << "Printed!" << endl;
}

void AndreiUtils::convertDepthToMillimetersUInt16(const Mat *depthMat, Mat &output) {
    depthMat->convertTo(output, CV_16U, 1000);
}

void AndreiUtils::convertDepthToMillimetersUInt16(Mat *depthMat) {
    depthMat->convertTo(*depthMat, CV_16U, 1000);
}

void AndreiUtils::convertDepthToMetersDouble64(const Mat *depthMat, Mat &output) {
    depthMat->convertTo(output, CV_64F, 0.001);
}

void AndreiUtils::convertDepthToMetersDouble64(Mat *depthMat) {
    depthMat->convertTo(*depthMat, CV_64F, 0.001);
}

void AndreiUtils::displayTextOnOpenCVMat(Mat &image, const string &text, Point topLeftCorner, float fontSize,
                                         const Scalar &textColor, int fontFace, int lineType) {
    int rowTextSize = (int) (fontSize * 20) + 5;
    int thickness = (int) (2 * fontSize);
    vector<string> textLines = splitString(text, "\n");
    for (const auto &textLine: textLines) {
        topLeftCorner.y += rowTextSize;
        cv::putText(image, textLine, topLeftCorner, fontFace, fontSize, textColor, thickness, lineType);
    }
}

void AndreiUtils::recoverPoseFrom2dAnd3dPoints(
        cv::Mat &tVec, cv::Mat &rVec, const vector<cv::Point2f> &points2d, const vector<cv::Point3f> &points3d,
        double fx, double fy, double ppx, double ppy, float distortionCoefficients[5]) {
    cv::Matx33d cameraMatrix(fx, 0, ppx, 0, fy, ppy, 0, 0, 1);
    cv::Vec<float, 5> distortionParameters;
    if (distortionCoefficients != nullptr) {
        distortionParameters = cv::Vec<float, 5>(distortionCoefficients);
    }

    cv::solvePnP(points3d, points2d, cameraMatrix, distortionParameters, rVec, tVec, false, cv::SOLVEPNP_EPNP);
    cv::Matx33d r;
    cv::Rodrigues(rVec, r);
}

void AndreiUtils::recoverPoseFrom2dAnd3dPoints(
        cv::Mat &tVec, cv::Matx33d &rMat, const vector<cv::Point2f> &points2d, const vector<cv::Point3f> &points3d,
        double fx, double fy, double ppx, double ppy, float distortionCoefficients[5]) {
    cv::Mat rVec;
    recoverPoseFrom2dAnd3dPoints(tVec, rVec, points2d, points3d, fx, fy, ppx, ppy, distortionCoefficients);
    cv::Rodrigues(rVec, rMat);
}

void AndreiUtils::recoverPoseFrom2dAnd3dPoints(
        cv::Mat &tVec, cv::Mat &rVec, const vector<cv::Point2f> &points2d, const vector<cv::Point3f> &points3d,
        const CameraIntrinsicParameters &intrinsics) {
    recoverPoseFrom2dAnd3dPoints(tVec, rVec, points2d, points3d, intrinsics.fx, intrinsics.fy, intrinsics.ppx,
                                 intrinsics.ppy, intrinsics.distortionCoefficients);
}

void AndreiUtils::recoverPoseFrom2dAnd3dPoints(
        cv::Mat &tVec, cv::Matx33d &rMat, const vector<cv::Point2f> &points2d, const vector<cv::Point3f> &points3d,
        const CameraIntrinsicParameters &intrinsics) {
    recoverPoseFrom2dAnd3dPoints(tVec, rMat, points2d, points3d, intrinsics.fx, intrinsics.fy, intrinsics.ppx,
                                 intrinsics.ppy, intrinsics.distortionCoefficients);
}
