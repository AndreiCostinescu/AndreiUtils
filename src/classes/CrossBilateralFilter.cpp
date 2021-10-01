//
// Created by Andrei on 29-Dec-20.
//

#include <AndreiUtils/classes/CrossBilateralFilter.h>

using namespace AndreiUtils;
using namespace cv;
using namespace std;

CrossBilateralFilter::CrossBilateralFilter(unsigned int windowSize) :
        windowSize(windowSize), spatial({windowSize, windowSize}), cross(1) {}

CrossBilateralFilter::~CrossBilateralFilter() = default;

void CrossBilateralFilter::filter(const float posX, const float posY, const Mat &depthData, float &resX,
                                  float &resY) const {
    cv::Point2d res, newP;
    cv::Point2i p((int) round(posX), (int) round(posY));
    double coefficientSum = 0.0, coefficient, depth, refDepth = depthData.at<double>(p.x, p.y);
    int shift = (int) this->windowSize / 2;
    int startI = -min(shift, p.x), endI = min(shift, depthData.rows - p.x);
    int startJ = -min(shift, p.y), endJ = min(shift, depthData.cols - p.y);
    for (int i = startI; i < endI; i++) {
        for (int j = startJ; j < endJ; j++) {
            newP.x = posX + (double) i;
            newP.y = posY + (double) j;
            depth = depthData.at<double>((int) round(newP.x), (int) round(newP.y));
            coefficient = this->spatial.coefficient({(double) (i + shift), (double) (j + shift)}) *
                          this->cross.coefficient(depth - refDepth);
            res += newP * coefficient;
            coefficientSum += coefficient;
        }
    }
    res /= coefficientSum;
    resX = res.x;
    resY = res.y;
}
