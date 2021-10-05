//
// Created by Andrei on 29-Dec-20.
//

#ifndef ANDREIUTILS_CROSSBILATERALFILTER_H
#define ANDREIUTILS_CROSSBILATERALFILTER_H

#include <AndreiUtils/classes/GaussianKernel.h>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    class CrossBilateralFilter {
    public:
        explicit CrossBilateralFilter(unsigned int windowSize);

        virtual ~CrossBilateralFilter();

        void filter(float posX, float posY, const cv::Mat &depthData, float &resX, float &resY) const;

        void filter(float posX, float posY, const Eigen::MatrixXd &depthData, float &resX, float &resY) const;

    private:
        unsigned int windowSize;
        const AndreiUtils::GaussianKernel spatial, cross;
    };
}

#endif //ANDREIUTILS_CROSSBILATERALFILTER_H
