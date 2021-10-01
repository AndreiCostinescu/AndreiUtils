//
// Created by Andrei on 29-Dec-20.
//

#ifndef ANDREIUTILS_GAUSSIANKERNEL_H
#define ANDREIUTILS_GAUSSIANKERNEL_H

#include <map>
#include <vector>

namespace AndreiUtils {
/*
 * Class to be used to compute the coefficients of the gaussian kernel
 */
    class GaussianKernel {
    public:
        explicit GaussianKernel();

        explicit GaussianKernel(unsigned int dimension);

        explicit GaussianKernel(const std::vector<unsigned int> &size);

        virtual ~GaussianKernel();

        double coefficient(double x);

        double coefficient(double x) const;

        double coefficient(const std::vector<double> &x);

        double coefficient(const std::vector<double> &x) const;

        double coefficientSum();

    private:
        double getCoefficient(const std::vector<double> &x, bool forceComputing = false) const;

        void initializeComputedCoefficients(bool verbose = false);

        unsigned int dimension;
        int maxIndex;
        std::vector<unsigned int> windowSize;
        std::map<int, double> computedCoefficientValues;
    };
}

#endif //ANDREIUTILS_GAUSSIANKERNEL_H
