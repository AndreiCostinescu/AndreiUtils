//
// Created by andrei on 19.10.21.
//

#ifndef ANDREIUTILS_CROSSBILATERALFILTEREIGEN_H
#define ANDREIUTILS_CROSSBILATERALFILTEREIGEN_H

#include <AndreiUtils/classes/CrossBilateralFilter.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    class CrossBilateralFilterEigen : public CrossBilateralFilter<Eigen::MatrixXd> {
    public:
        explicit CrossBilateralFilterEigen(unsigned int windowSize);

        void filter(float posX, float posY, const Eigen::MatrixXd &depthData, float &resX, float &resY) const override;
    };
}

#endif //ANDREIUTILS_CROSSBILATERALFILTEREIGEN_H
