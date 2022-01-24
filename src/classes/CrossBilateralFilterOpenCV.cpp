//
// Created by andrei on 19.10.21.
//

#include <AndreiUtils/classes/CrossBilateralFilterOpenCV.h>

using namespace AndreiUtils;
using namespace cv;

CrossBilateralFilterOpenCV::CrossBilateralFilterOpenCV(unsigned int windowSize) :
        CrossBilateralFilter<Mat>(windowSize) {}

void CrossBilateralFilterOpenCV::filter(float posX, float posY, const Mat &depthData, float &resX, float &resY) const {
    if (depthData.rows <= (int) posY || posY < 0 || depthData.cols <= (int) posX || posX < 0) {
        return;
    }
    Point2d res, newP;
    Point2i p((int) round(posX), (int) round(posY));
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
    resX = (float) res.x;
    resY = (float) res.y;
}


