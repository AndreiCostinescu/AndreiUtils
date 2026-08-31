// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
} // namespace AndreiUtils

#endif // ANDREIUTILS_GAUSSIANKERNEL_H
