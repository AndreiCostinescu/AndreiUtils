//
// Created by Andrei on 26.11.20.
//

#ifndef ANDREIUTILS_UTILSOPENCV_H
#define ANDREIUTILS_UTILSOPENCV_H

#include <AndreiUtils/enums/RotationType.h>
#include <Eigen/Dense>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <vector>

namespace AndreiUtils {
    size_t matByteSize(const cv::Mat &mat);

    void imageRotation(cv::Mat *image, RotationType rotation = RotationType::NO_ROTATION);

    void matWriteBinary(std::ofstream *fs, const cv::Mat &mat);

    bool matReadBinary(std::ifstream *fs, cv::Mat *result);

    void printImage(cv::Mat *image, const char *title, bool verbose = false);

    void printImages(const std::vector<cv::Mat *> &images, const std::vector<std::string> &titles);

    void convertDepthToMillimetersUInt16(const cv::Mat *depthMat, cv::Mat &output);

    void convertDepthToMillimetersUInt16(cv::Mat *depthMat);

    void convertDepthToMetersDouble64(const cv::Mat *depthMat, cv::Mat &output);

    void convertDepthToMetersDouble64(cv::Mat *depthMat);
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
