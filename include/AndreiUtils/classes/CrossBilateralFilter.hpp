//
// Created by Andrei on 29-Dec-20.
//

#ifndef ANDREIUTILS_CROSSBILATERALFILTER_HPP
#define ANDREIUTILS_CROSSBILATERALFILTER_HPP

#include <AndreiUtils/classes/GaussianKernel.h>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    template<class T>
    class CrossBilateralFilter {
    public:
        explicit CrossBilateralFilter(unsigned int windowSize) :
                windowSize(windowSize), spatial({windowSize, windowSize}), cross(1) {}

        virtual ~CrossBilateralFilter() = default;

        virtual void filter(float posX, float posY, const T &depthData, float &resX, float &resY) const = 0;

    protected:
        unsigned int windowSize;
        const AndreiUtils::GaussianKernel spatial, cross;
    };
}

#endif //ANDREIUTILS_CROSSBILATERALFILTER_HPP
