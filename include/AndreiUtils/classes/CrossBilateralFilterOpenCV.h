//
// Created by andrei on 19.10.21.
//

#ifndef ANDREIUTILS_CROSSBILATERALFILTEROPENCV_H
#define ANDREIUTILS_CROSSBILATERALFILTEROPENCV_H

#include <opencv2/opencv.hpp>
#include <AndreiUtils/classes/CrossBilateralFilter.hpp>

namespace AndreiUtils {
    class CrossBilateralFilterOpenCV : public CrossBilateralFilter<cv::Mat> {
    public:
        explicit CrossBilateralFilterOpenCV(unsigned int windowSize);

        void filter(float posX, float posY, const cv::Mat &depthData, float &resX, float &resY) const override;
    };
}

#endif //ANDREIUTILS_CROSSBILATERALFILTEROPENCV_H
