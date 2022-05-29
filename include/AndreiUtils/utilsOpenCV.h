//
// Created by Andrei on 26.11.20.
//

#ifndef ANDREIUTILS_UTILSOPENCV_H
#define ANDREIUTILS_UTILSOPENCV_H

#include <AndreiUtils/classes/camera/CameraIntrinsicParameters.h>
#include <AndreiUtils/enums/RotationType.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <vector>

namespace AndreiUtils {
    size_t matByteSize(const cv::Mat &mat);

    void imageRotation(cv::Mat *image, RotationType rotation = RotationType::NO_ROTATION);

    uchar *copyMatData(const cv::Mat &image);

    void matWriteBinary(std::ofstream *fs, const cv::Mat &mat);

    bool matReadBinary(std::ifstream *fs, cv::Mat *result);

    void displayImage(const cv::Mat &image, const char *title, bool verbose = false);

    void displayImage(const cv::Mat *image, const char *title, bool verbose = false);

    void displayImages(const std::vector<cv::Mat> &images, const std::vector<std::string> &titles);

    void displayImages(const std::vector<const cv::Mat *> &images, const std::vector<std::string> &titles);

    void convertDepthToMillimetersUInt16(const cv::Mat *depthMat, cv::Mat &output);

    void convertDepthToMillimetersUInt16(cv::Mat *depthMat);

    void convertDepthToMetersDouble64(const cv::Mat *depthMat, cv::Mat &output);

    void convertDepthToMetersDouble64(cv::Mat *depthMat);

    void displayTextOnOpenCVMat(cv::Mat &image, const std::string &text, cv::Point topLeftCorner, float fontSize = 0.6,
                                const cv::Scalar &textColor = {255, 255, 255}, int fontFace = cv::FONT_HERSHEY_SIMPLEX,
                                int lineType = cv::LINE_AA);

    void recoverPoseFrom2dAnd3dPoints(
            cv::Mat &tVec, cv::Mat &rVec, const std::vector<cv::Point2f> &points2d,
            const std::vector<cv::Point3f> &points3d, double fx, double fy, double ppx, double ppy,
            float distortionCoefficients[5]);

    void recoverPoseFrom2dAnd3dPoints(
            cv::Mat &tVec, cv::Matx33d &rMat, const std::vector<cv::Point2f> &points2d,
            const std::vector<cv::Point3f> &points3d, double fx, double fy, double ppx, double ppy,
            float distortionCoefficients[5]);

    void recoverPoseFrom2dAnd3dPoints(
            cv::Mat &tVec, cv::Mat &rVec, const std::vector<cv::Point2f> &points2d,
            const std::vector<cv::Point3f> &points3d, const CameraIntrinsicParameters &intrinsics);

    void recoverPoseFrom2dAnd3dPoints(
            cv::Mat &tVec, cv::Matx33d &rMat, const std::vector<cv::Point2f> &points2d,
            const std::vector<cv::Point3f> &points3d, const CameraIntrinsicParameters &intrinsics);
}

namespace cv {
    template<typename T>
    void write(cv::FileStorage &fs, const std::string &name, const std::complex<T> &x) {
        fs << "{";
        fs << "r" << x.real();
        fs << "i" << x.imag();
        fs << "}";
    }

    template<typename T>
    void read(const cv::FileNode &node, std::complex<T> &x, const std::complex<T> &default_value = std::complex<T>()) {
        if (node.empty()) {
            x = default_value;
        } else {
            x.real((T) node["r"]);
            x.imag((T) node["i"]);
        }
    }

    template<typename T>
    void write(cv::FileStorage &fs, const std::string &name, const std::vector<std::complex<T>> &x) {
        fs << "{";
        fs << "size" << (int) x.size();
        for (int i = 0; i < x.size(); i++) {
            fs << "elem" + std::to_string(i) << x[i];
        }
        fs << "}";
    }

    template<typename T>
    void read(const cv::FileNode &node, std::vector<std::complex<T>> &x,
              const std::vector<std::complex<T>> &default_value = std::vector<std::complex<T>>()) {
        if (node.empty()) {
            x = default_value;
        } else {
            x.resize((int) node["size"]);
            for (int i = 0; i < x.size(); i++) {
                cv::read(node["elem" + std::to_string(i)], x[i]);
            }
        }
    }
}

#endif //ANDREIUTILS_UTILSOPENCV_H
