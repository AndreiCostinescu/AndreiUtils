/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei on 29-Dec-20.
//

#include <AndreiUtils/classes/GaussianKernel.h>
#include <cassert>
#include <cmath>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

GaussianKernel::GaussianKernel() : dimension(-1), maxIndex(0) {}

GaussianKernel::GaussianKernel(unsigned int dimension) : dimension(dimension), maxIndex(0) {}

GaussianKernel::GaussianKernel(const std::vector<unsigned int> &size) :
    dimension(size.size()), maxIndex(1), windowSize(size) {
    for (const auto &dimValue: this->windowSize) {
        // if it's an even number
        if ((dimValue & 1) == 0) {
            throw std::runtime_error("The gaussian kernel's dimensions can not be even!");
        }
    }
    this->initializeComputedCoefficients();
}

GaussianKernel::~GaussianKernel() = default;

double GaussianKernel::coefficient(double x) {
    // assume that the x is 0-mean and 1-std_var
    if (this->dimension < 1) {
        this->dimension = 1;
        this->initializeComputedCoefficients();
    }
    return this->getCoefficient({x});
}

double GaussianKernel::coefficient(double x) const { return this->getCoefficient({x}); }

double GaussianKernel::coefficient(const std::vector<double> &x) {
    if (this->dimension < 1) {
        this->dimension = x.size();
        this->initializeComputedCoefficients();
    }
    return this->getCoefficient(x);
}

double GaussianKernel::coefficient(const std::vector<double> &x) const { return this->getCoefficient(x); }

double GaussianKernel::coefficientSum() {
    if (this->dimension < 1 || this->windowSize.empty()) {
        throw std::runtime_error("Can only sum the coefficients if the windowSize is set!");
    }
    double sum = 0.0;
    for (const auto &coefficientValue: this->computedCoefficientValues) {
        sum += coefficientValue.second;
    }
    return sum;
}

double GaussianKernel::getCoefficient(const std::vector<double> &x, bool forceComputing) const {
    // assume that the x is 0-mean and 1-std_var
    assert(this->dimension == x.size());
    if (!forceComputing && !this->windowSize.empty()) {
        int index = 0, cpMaxIndex = this->maxIndex;
        for (int i = 0; i < x.size(); i++) {
            cpMaxIndex /= this->windowSize[i];
            index += (int) x[i] * cpMaxIndex;
        }
        assert(index < this->maxIndex);
        return this->computedCoefficientValues.at(index);
    }

    double sum = 0.0;
    for (const auto &val: x) {
        sum += val * val;
    }
    return exp(-0.5 * sum);
}

void GaussianKernel::initializeComputedCoefficients(bool verbose) {
    std::vector<double> x(this->dimension);
    int cpMaxIndex, index, cpIndex;
    for (const auto &dimValue: this->windowSize) {
        this->maxIndex *= dimValue;
    }
    for (index = 0; index < this->maxIndex; index++) {
        cpIndex = index;
        cpMaxIndex = this->maxIndex;
        for (int dimIndex = 0; dimIndex < this->dimension; dimIndex++) {
            cpMaxIndex /= this->windowSize[dimIndex];
            x[dimIndex] = (double) ((cpIndex / cpMaxIndex) - (this->windowSize[dimIndex] / 2));
            cpIndex %= cpMaxIndex;
        }
        this->computedCoefficientValues[index] = this->getCoefficient(x, true);
        if (verbose) {
            std::cout << "Index conversion: " << index << " -> ";
            for (int i = 0; i < this->dimension; i++) {
                std::cout << x[i] << ", ";
            }
            std::cout << ": " << this->computedCoefficientValues[index] << std::endl;
        }
    }
}
