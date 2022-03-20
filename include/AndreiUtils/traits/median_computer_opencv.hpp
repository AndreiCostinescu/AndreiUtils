//
// Created by Andrei on 20-Mar-22.
//

#ifndef ANDREIUTILS_MEDIAN_COMPUTER_OPENCV_HPP
#define ANDREIUTILS_MEDIAN_COMPUTER_OPENCV_HPP

#include <AndreiUtils/traits/median_computer.hpp>
#include <opencv2/opencv.hpp>

template<typename Scalar>
struct median_computer<cv::Point_<Scalar>> {
    static cv::Point_<Scalar> medianComputer(std::vector<cv::Point_<Scalar>> v) {
        std::vector<Scalar> dimensionValues(v.size());
        cv::Point_<Scalar> res(0, 0);
        if (values.empty()) {
            return res;
        }
        Scalar value;
        for (int dim = 0; dim < 2; dim++) {
            for (int i = 0; i < values.size(); i++) {
                if (dim == 0) {
                    value = values[i].x;
                } else if (dim == 1) {
                    value = values[i].y;
                }
                dimensionValues[i] = value;
            }
            value = median_computer<Scalar>::medianComputer(dimensionValues);
            if (dim == 0) {
                res.x = value;
            } else if (dim == 1) {
                res.y = value;
            }
        }
        return res;
    }
};

template<typename Scalar>
struct median_computer<cv::Point3_<Scalar>> {
    static cv::Point3_<Scalar> medianComputer(std::vector<cv::Point3_<Scalar>> v) {
        std::vector<Scalar> dimensionValues(v.size());
        cv::Point3_<Scalar> res(0, 0, 0);
        if (values.empty()) {
            return res;
        }
        Scalar value;
        for (int dim = 0; dim < 3; dim++) {
            for (int i = 0; i < values.size(); i++) {
                if (dim == 0) {
                    value = values[i].x;
                } else if (dim == 1) {
                    value = values[i].y;
                } else if (dim == 2) {
                    value = values[i].z;
                }
                dimensionValues[i] = value;
            }
            value = median_computer<Scalar>::medianComputer(dimensionValues);
            if (dim == 0) {
                res.x = value;
            } else if (dim == 1) {
                res.y = value;
            } else if (dim == 2) {
                res.z = value;
            }
        }
        return res;
    }
};

#endif //ANDREIUTILS_MEDIAN_COMPUTER_OPENCV_HPP
