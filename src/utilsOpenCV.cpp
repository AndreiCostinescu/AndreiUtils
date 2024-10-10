//
// Created by Andrei on 26.11.20.
//

#include <AndreiUtils/utilsOpenCV.h>
#include <AndreiUtils/utilsString.h>

#ifdef WITH_OPENMP

#include <AndreiUtils/utilsOpenMP.hpp>

#endif

using namespace cv;
using namespace std;

bool AndreiUtils::matEqual(Mat const &m1, Mat const &m2) {
    if (m1.size != m2.size) {
        return false;
    }
    return cv::sum(m1 != m2) == cv::Scalar(0,0,0,0);
}

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
#ifdef WITH_OPENMP
        fastMemCopy(dataPtr, mat.ptr(0), (mat.dataend - mat.datastart));
#else
        memcpy(dataPtr, mat.ptr(0), (mat.dataend - mat.datastart) * sizeof(uchar));
#endif
    } else {
        int rowSize = CV_ELEM_SIZE(mat.type()) * mat.cols;
        for (int r = 0; r < mat.rows; ++r) {
#ifdef WITH_OPENMP
            fastMemCopy(dataPtr + r * rowSize, mat.ptr(r), rowSize);
#else
            memcpy(dataPtr + r * rowSize, mat.ptr(r), rowSize * sizeof(uchar));
#endif
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

void AndreiUtils::convertDepthToMillimetersUInt16(Mat const *depthMat, Mat &output) {
    depthMat->convertTo(output, CV_16U, 1000);
}

void AndreiUtils::convertDepthToMillimetersUInt16(Mat *depthMat) {
    depthMat->convertTo(*depthMat, CV_16U, 1000);
}

void AndreiUtils::convertDepthToMetersDouble64(Mat const *depthMat, Mat &output) {
    depthMat->convertTo(output, CV_64F, 0.001);
}

void AndreiUtils::convertDepthToMetersDouble64(Mat *depthMat) {
    depthMat->convertTo(*depthMat, CV_64F, 0.001);
}

void AndreiUtils::displayTextOnOpenCVMat(Mat &image, string const &text, Point topLeftCorner, float fontSize,
                                         Scalar const &textColor, int fontFace, int lineType, int lineContentCap) {
    int rowTextSize = (int) (fontSize * 20) + 5;
    int thickness = (int) (2 * fontSize);
    vector<string> textLines = splitString(text, "\n");
    for (const auto &textLine: textLines) {
        topLeftCorner.y += rowTextSize;
        if (lineContentCap <= 0) {
            cv::putText(image, textLine, topLeftCorner, fontFace, fontSize, textColor, thickness, lineType);
        } else {
            auto lineTextSplit = splitString(textLine, " ");
            if (lineTextSplit.size() < 2) {
                cv::putText(image, textLine, topLeftCorner, fontFace, fontSize, textColor, thickness, lineType);
            } else {
                int nrParts = (int) lineTextSplit.size(), partCount = 0;
                std::string contentUntilNow;
                for (int partIndex = 0; partIndex < nrParts; ++partIndex) {
                    if (partCount == 0 ||
                        contentUntilNow.size() + lineTextSplit[partIndex].size() + (!partCount) <= lineContentCap) {
                        if (partCount != 0) {
                            contentUntilNow += " ";
                        }
                        contentUntilNow += lineTextSplit[partIndex];
                        ++partCount;
                    } else {
                        cv::putText(image, contentUntilNow, topLeftCorner, fontFace, fontSize, textColor, thickness,
                                    lineType);
                        topLeftCorner.y += rowTextSize;
                        contentUntilNow = AndreiUtils::tab * 4 + lineTextSplit[partIndex];
                        partCount = 1;
                    }
                }
                assert(partCount != 0);
                cv::putText(image, contentUntilNow, topLeftCorner, fontFace, fontSize, textColor, thickness, lineType);
            }
        }
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

Point2i AndreiUtils::castFromPointDouble(Point2d const &p) {
    return {int(p.x), int(p.y)};
}

cv::Mat AndreiUtils::addColorBorderAroundImage(cv::Mat const &img, cv::Scalar const &borderColor, int borderWidth) {
    cv::Mat res;
    cv::copyMakeBorder(img, res, borderWidth, borderWidth, borderWidth, borderWidth, BORDER_CONSTANT, borderColor);
    // cv::Mat res(img.rows + 2 * borderWidth, img.cols + 2 * borderWidth, CV_8UC3, borderColor);
    // img.copyTo(res.colRange(borderWidth, borderWidth + img.cols).rowRange(borderWidth, borderWidth + img.rows));
    return res;
}

cv::Mat AndreiUtils::addColorBorderAroundImage(cv::Mat const &img, cv::Scalar const &borderColor, int topWidth,
                                               int rightWidth, int bottomWidth, int leftWidth) {
    cv::Mat res;
    cv::copyMakeBorder(img, res, topWidth, bottomWidth, leftWidth, rightWidth, BORDER_CONSTANT, borderColor);
    return res;
}
