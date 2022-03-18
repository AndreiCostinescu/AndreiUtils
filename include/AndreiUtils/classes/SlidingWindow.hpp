//
// Created by andrei on 03.12.20.
//

#ifndef ANDREIUTILS_SLIDINGWINDOW_HPP
#define ANDREIUTILS_SLIDINGWINDOW_HPP

#include <AndreiUtils/enums/InvalidValuesHandlingMode.h>
#include <AndreiUtils/utils.hpp>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

namespace AndreiUtils {
    template<class T>
    class SlidingWindow {
    public:
        explicit SlidingWindow(unsigned size = 0) : data(size), index(0), size(size), dataSize(0) {}

        void addData(T newData) {
            assert(this->size > 0);
            this->data[this->index] = std::move(newData);
            this->index += 1;
            if (this->index == this->data.size()) {
                this->index = 0;
            }
            if (this->dataSize < this->size) {
                this->dataSize++;
            }
            // std::cout << this->index << "; " << this->dataSize << "; " << this->size << "; " << std::endl;
        }

        void clear() {
            this->index = 0;
            this->dataSize = 0;
        }

        T convolve(const std::vector<double> &parameters) const {
            bool average = false;
            if (parameters.size() != this->dataSize) {
                average = true;
            }
            T res;
            double parameter;
            for (int i = 1; i <= this->dataSize; i++) {
                parameter = (average) ? 1.0 / this->size : parameters[this->dataSize - i];
                if (i == 0) {
                    res = parameter * this->data[(this->index + this->size - i) % this->size];
                } else {
                    res += parameter * this->data[(this->index + this->size - i) % this->size];
                }
            }
            return res;
        }

        T getMedian() const {
            return AndreiUtils::median(this->getDataInCorrectOrder());
        }

        T getAverage() const {
            return AndreiUtils::average(this->getDataInCorrectOrder());
        }

        std::vector<T> &getData() {
            return this->data;
        }

    protected:
        std::vector<T> getDataInCorrectOrder() const {
            std::vector<T> a(this->dataSize);
            for (unsigned int i = this->dataSize; i >= 1; i--) {
                a[this->dataSize - i] = this->data[(this->index + this->size - i) % this->size];
            }
            return a;
        }

        std::vector<T> data;
        unsigned index, size, dataSize;
    };

    template<class T>
    class SlidingWindowWithInvalidValues : SlidingWindow<T> {
    public:
        explicit SlidingWindowWithInvalidValues(unsigned size = 0) : SlidingWindow<T>(size), validData(size) {}

        void addData(T newData, bool valid = true) {
            assert(this->size > 0);
            this->validData[this->index] = valid;
            SlidingWindow<T>::addData(newData);
        }

        T convolve(const std::vector<double> &parameters, InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            bool average = false;
            if (parameters.size() != this->dataSize) {
                average = true;
            }
            T res;
            double parameter;
            int dataIndex, nrValidData = 0;
            for (int i = 1; i <= this->dataSize; i++) {
                dataIndex = (this->index + this->size - i) % this->size;
                if (!this->validData[dataIndex] &&
                    invalidValuesHandlingMode != InvalidValuesHandlingMode::IGNORE_INVALID) {
                    if (invalidValuesHandlingMode == InvalidValuesHandlingMode::SKIP_INVALID) {
                        continue;
                    } else {
                        throw std::runtime_error("FAIL_UPON_INVALID mode selected");
                    }
                }
                parameter = (average) ? 1.0 : parameters[this->dataSize - i];
                if (i == 0) {
                    res = parameter * this->data[dataIndex];
                } else {
                    res += parameter * this->data[dataIndex];
                }
                nrValidData++;
            }
            if (average && nrValidData > 0) {
                res = res / nrValidData;
            }
            return res;
        }

        T getMedian(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            return AndreiUtils::median(this->getDataInCorrectOrder(invalidValuesHandlingMode));
        }

        T getAverage(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            return AndreiUtils::average(this->getDataInCorrectOrder(invalidValuesHandlingMode));
        }

        std::vector<bool> &getValidFlags() {
            return this->validData;
        }

    protected:
        std::vector<T> getDataInCorrectOrder(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            std::vector<T> a(this->dataSize);
            int nrValidValues = 0, nrInvalidValues = 0, dataIndex;
            for (unsigned int i = this->dataSize; i >= 1; i--) {
                dataIndex = (this->index + this->size - i) % this->size;
                if (this->validData[dataIndex] ||
                    invalidValuesHandlingMode == InvalidValuesHandlingMode::IGNORE_INVALID) {
                    a[this->dataSize - i - nrInvalidValues] = this->data[dataIndex];
                    nrValidValues++;
                } else if (invalidValuesHandlingMode == InvalidValuesHandlingMode::SKIP_INVALID) {
                    nrInvalidValues++;
                } else {
                    throw std::runtime_error("FAIL_UPON_INVALID mode selected");
                }
            }
            a.resize(nrValidValues);
            return a;
        }

        std::vector<bool> validData;
    };
}

#endif //ANDREIUTILS_SLIDINGWINDOW_HPP
