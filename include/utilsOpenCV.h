//
// Created by andrei on 26.11.20.
//

#ifndef ANDREIUTILS_UTILSOPENCV_H
#define ANDREIUTILS_UTILSOPENCV_H

#include <fstream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <enums/RotationType.h>

void imageRotation(cv::Mat *image, RotationType rotation = RotationType::NO_ROTATION);

void matWriteBinary(std::ofstream *fs, const cv::Mat &mat, const unsigned long long &index);

bool matReadBinary(std::ifstream *fs, cv::Mat *result);

void printImage(cv::Mat *image, const char *title, bool verbose);

void printImages(const std::vector<cv::Mat *> &images, const std::vector<std::string> &titles);

#endif //ANDREIUTILS_UTILSOPENCV_H
