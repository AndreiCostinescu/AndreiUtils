//
// Created by andrei on 26.11.20.
//

#ifndef ANDREIUTILS_UTILSOPENCV_H
#define ANDREIUTILS_UTILSOPENCV_H

#include <AndreiUtils/enums/RotationType.h>
#include <Eigen/Dense>
#include <fstream>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    void imageRotation(cv::Mat *image, RotationType rotation = RotationType::NO_ROTATION);

    void matWriteBinary(std::ofstream *fs, const cv::Mat &mat);

    bool matReadBinary(std::ifstream *fs, cv::Mat *result);

    void printImage(cv::Mat *image, const char *title, bool verbose = false);

    void printImages(const std::vector<cv::Mat *> &images, const std::vector<std::string> &titles);
}

#endif //ANDREIUTILS_UTILSOPENCV_H
