//
// Created by andrei on 03.12.20.
//

#ifndef ANDREIUTILS_SLIDINGWINDOW_HPP
#define ANDREIUTILS_SLIDINGWINDOW_HPP

#include <AndreiUtils/enums/InvalidValuesHandlingMode.h>
#include <AndreiUtils/traits/get_vector_type_for_convolution.hpp>
#include <AndreiUtils/traits/median_computer.hpp>
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

        unsigned getSize() const {
            return this->size;
        }

        unsigned getDataSize() const {
            return this->dataSize;
        }

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

        T convolve(const std::vector<typename get_vector_type_for_convolution<T>::type> &parameters) const {
            if (parameters.size() != this->dataSize) {
                throw std::runtime_error("Can not convolve with different parameter sizes!");
            }
            T res, value;
            int dataIndex;
            // travel backwards because this->index points to the next free position => this->index - 1 is the last elem
            for (int i = 1; i <= this->dataSize; i++) {
                dataIndex = (this->index + this->size - i) % this->size;
                value = parameters[this->dataSize - i] * this->data[dataIndex];
                if (i == 1) {
                    res = value;
                } else {
                    res += value;
                }
            }
            return res;
        }

        T getMedian() const {
            return median_computer<T>::medianComputer(this->getDataInCorrectOrder());
        }

        T getAverage() const {
            return AndreiUtils::average(this->getDataInCorrectOrder());
        }

        T &getLatest() {
            return this->data[this->latestIndex()];
        }

        T getLatest() const {
            return this->data[this->latestIndex()];
        }

        T &getEarliest() {
            return this->data[this->earliestIndex()];
        }

        T getEarliest() const {
            return this->data[this->earliestIndex()];
        }

        std::vector<T> &getData() {
            return this->data;
        }

    protected:
        size_t latestIndex() const {
            if (this->dataSize == 0) {
                throw std::runtime_error("There is no latest element because there is no element in the SlidingWindow");
            }
            return (this->index + this->size - 1) % this->size;
        }

        size_t earliestIndex() const {
            if (this->dataSize == 0) {
                throw std::runtime_error(
                        "There is no earliest element because there is no element in the SlidingWindow");
            }
            return (this->index + this->size - this->dataSize) % this->size;
        }

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
    class SlidingWindowWithInvalidValues : public SlidingWindow<T> {
    public:
        explicit SlidingWindowWithInvalidValues(unsigned size = 0) : SlidingWindow<T>(size), validData(size) {}

        void addData(T newData, bool valid = true) {
            assert(this->size > 0);
            this->validData[this->index] = valid ? 1 : 0;
            SlidingWindow<T>::addData(newData);
        }

        T convolve(const std::vector<typename get_vector_type_for_convolution<T>::type> &parameters,
                   InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            if (parameters.size() != this->dataSize) {
                throw std::runtime_error("Can not convolve with different parameter sizes!");
            }
            T res, value;
            int dataIndex;
            // travel backwards because this->index points to the next free position => this->index - 1 is the last elem
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
                value = parameters[this->dataSize - i] * this->data[dataIndex];
                if (i == 0) {
                    res = value;
                } else {
                    res += value;
                }
            }
            return res;
        }

        T getMedian(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            return median_computer<T>::medianComputer(this->getDataInCorrectOrder(invalidValuesHandlingMode));
        }

        T getAverage(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            return AndreiUtils::average(this->getDataInCorrectOrder(invalidValuesHandlingMode));
        }

        void setLatestValid(bool valid) {
            this->validData[this->latestIndex()] = valid ? 1 : 0;
        }

        bool getLatestValid() const {
            return (this->validData[this->latestIndex()] != 0);
        }

        void getEarliestValid(bool valid) {
            this->validData[this->earliestIndex()] = valid ? 1 : 0;
        }

        bool getEarliestValid() const {
            return (this->validData[this->earliestIndex()] != 0);
        }

        std::vector<uint8_t> &getValidFlags() {
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

        std::vector<uint8_t> validData;
    };
}

#endif //ANDREIUTILS_SLIDINGWINDOW_HPP
