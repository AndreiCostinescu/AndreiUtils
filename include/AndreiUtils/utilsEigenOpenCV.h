//
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSEIGENOPENCV_H
#define ANDREIUTILS_UTILSEIGENOPENCV_H

#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    Eigen::Array3f cvPointToEigenArray(const cv::Point3f &point);

    cv::Point3f eigenArrayToCVPoint(const Eigen::Array3f &array);

    void writeEigenArray(cv::FileStorage &fs, const Eigen::ArrayXf &x);

    void readEigenArray(const cv::FileNode &node, Eigen::ArrayXf &x);

    void write(cv::FileStorage &fs, const std::string &name, const Eigen::ArrayXf &x);

    void read(const cv::FileNode &node, Eigen::ArrayXf &x, const Eigen::ArrayXf &default_value = Eigen::ArrayXf());

    void write(cv::FileStorage &fs, const std::string &name, const Eigen::Array3f &x);

    void read(const cv::FileNode &node, Eigen::Array3f &x,
              const Eigen::Array3f &default_value = Eigen::Array3f::Zero());
}

#endif //ANDREIUTILS_UTILSEIGENOPENCV_H
