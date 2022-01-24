//
// Created by andrei on 19.10.21.
//

#include <AndreiUtils/classes/CrossBilateralFilterEigen.h>
#include <AndreiUtils/utils.hpp>

using namespace AndreiUtils;
using namespace Eigen;

CrossBilateralFilterEigen::CrossBilateralFilterEigen(unsigned int windowSize)
        : CrossBilateralFilter<MatrixXd>(windowSize) {}

void CrossBilateralFilterEigen::filter(float posX, float posY, const MatrixXd &depthData,
                                       float &resX, float &resY) const {
    if (depthData.rows() <= (int) posY || posY < 0 || depthData.cols() <= (int) posX || posX < 0) {
        return;
    }
    Vector2d res, newP;
    Vector2i p((int) round((double) posX), (int) round((double) posY));
    double coefficientSum = 0.0, coefficient, depth, refDepth = depthData(p.x(), p.y());
    int shift = (int) this->windowSize / 2;
    int startI = -fastMin(shift, p.x()), endI = fastMin(shift, (int) (depthData.rows() - p.x()));
    int startJ = -fastMin(shift, p.y()), endJ = fastMin(shift, (int) (depthData.cols() - p.y()));
    for (int i = startI; i < endI; i++) {
        for (int j = startJ; j < endJ; j++) {
            newP.x() = posX + (double) i;
            newP.y() = posY + (double) j;
            depth = depthData((int) round(newP.x()), (int) round(newP.y()));
            coefficient = this->spatial.coefficient({(double) (i + shift), (double) (j + shift)}) *
                          this->cross.coefficient(depth - refDepth);
            res += newP * coefficient;
            coefficientSum += coefficient;
        }
    }
    res /= coefficientSum;
    resX = (float) res.x();
    resY = (float) res.y();
}
