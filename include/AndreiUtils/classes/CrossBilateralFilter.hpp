//
// Created by Andrei on 29-Dec-20.
//

#ifndef ANDREIUTILS_CROSSBILATERALFILTER_HPP
#define ANDREIUTILS_CROSSBILATERALFILTER_HPP

#include <AndreiUtils/classes/GaussianKernel.h>
#include <AndreiUtils/traits/Container2D.hpp>
#include <cmath>
#include <functional>

namespace AndreiUtils {
    class CrossBilateralFilter {
    public:
        explicit CrossBilateralFilter(unsigned int windowSize) :
                windowSize(windowSize), spatial({windowSize, windowSize}), cross(1) {}

        virtual ~CrossBilateralFilter() = default;

        template<class T>
        void filter(float posX, float posY, const T &data, float &resX, float &resY) const {
            typedef Container2D<T> Container2D;
            static_assert(Container2D::isContainer2D, "Data is not a 2d container...");
            Container2D dataContainer(data);
            if (!dataContainer.checkPointInsideContainer(posY, posX)) {
                return;
            }
            double sumX = 0, sumY = 0, newX, newY;
            int pX = (int) std::round(posX), pY = (int) std::round(posY);
            double coefficientSum = 0.0, coefficient, dataPoint, refDataPoint = dataContainer.valueAt(pY, pX);
            int shift = (int) this->windowSize / 2;
            int startI = -std::min(shift, pX), endI = std::min(shift, dataContainer.height() - pX);
            int startJ = -std::min(shift, pY), endJ = std::min(shift, dataContainer.width() - pY);
            for (int i = startI; i < endI; i++) {
                for (int j = startJ; j < endJ; j++) {
                    newX = posX + (double) i;
                    newY = posY + (double) j;
                    dataPoint = dataContainer.valueAt((int) std::round(newY), (int) std::round(newX));
                    coefficient = this->spatial.coefficient({(double) (i + shift), (double) (j + shift)}) *
                                  this->cross.coefficient(dataPoint - refDataPoint);
                    sumX += newX * coefficient;
                    sumY += newY * coefficient;
                    coefficientSum += coefficient;
                }
            }
            resX = (float) (sumX / coefficientSum);
            resY = (float) (sumY / coefficientSum);
        }

        void filter(float posX, float posY, const std::function<float(int, int)> &getData, const int &dataHeight,
                    const int &dataWidth, float &resX, float &resY) const {
            if (0 > posY || posY >= (float) dataHeight || 0 > posX || posX >= (float) dataWidth) {
                return;
            }
            double sumX = 0, sumY = 0, newX, newY;
            int pX = (int) std::round(posX), pY = (int) std::round(posY);
            double coefficientSum = 0.0, coefficient, dataPoint, refDataPoint = getData(pX, pY);
            int shift = (int) this->windowSize / 2;
            int startI = -std::min(shift, pX), endI = std::min(shift, dataHeight - pX);
            int startJ = -std::min(shift, pY), endJ = std::min(shift, dataWidth - pY);
            for (int i = startI; i < endI; i++) {
                for (int j = startJ; j < endJ; j++) {
                    newX = posX + (double) i;
                    newY = posY + (double) j;
                    dataPoint = getData((int) std::round(newY), (int) std::round(newX));
                    coefficient = this->spatial.coefficient({(double) (i + shift), (double) (j + shift)}) *
                                  this->cross.coefficient(dataPoint - refDataPoint);
                    sumX += newX * coefficient;
                    sumY += newY * coefficient;
                    coefficientSum += coefficient;
                }
            }
            resX = (float) (sumX / coefficientSum);
            resY = (float) (sumY / coefficientSum);
        }

        void setWindowSize(unsigned int _filterWindowSize) {
            this->windowSize = _filterWindowSize;
        }

        unsigned int getWindowSize() const {
            return this->windowSize;
        }

    protected:
        unsigned int windowSize;
        const AndreiUtils::GaussianKernel spatial, cross;
    };
}

#endif //ANDREIUTILS_CROSSBILATERALFILTER_HPP
