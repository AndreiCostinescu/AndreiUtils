//
// Created by Andrei on 03.12.20.
//

#ifndef ANDREIUTILS_SLIDINGWINDOW_HPP
#define ANDREIUTILS_SLIDINGWINDOW_HPP

#include <AndreiUtils/enums/InvalidValuesHandlingMode.h>
#include <AndreiUtils/traits/get_vector_type_for_convolution.hpp>
#include <AndreiUtils/traits/median_computer.hpp>
#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsVector.hpp>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

namespace AndreiUtils {
    template<class T>
    class SlidingWindow {
    public:
        struct Iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;  // not ok...
            using value_type = T;
            using pointer = value_type *;
            using reference = value_type &;

            explicit Iterator(std::vector<value_type> *data, unsigned dataSize, unsigned dataIndex, unsigned index) :
                    container(data), containerDataSize(dataSize), index(index), containerIndex(dataIndex),
                    containerSize(data != nullptr ? data->size() : 0), containerStartIndex() {
                // this->index = fastMin(this->index, this->containerDataSize + 1);
                this->containerStartIndex =
                        (dataIndex + this->containerSize - this->containerDataSize + this->index) % this->containerSize;
            }

            reference operator*() const { return (*this->container)[this->containerStartIndex]; }

            pointer operator->() { return &(*this); }

            // Prefix increment
            Iterator &operator++() {
                /*
                if (this->index == this->containerDataSize) {
                    return *this;
                }
                //*/
                this->index++;
                this->containerStartIndex = (this->containerStartIndex + 1) % this->containerSize;
                return *this;
            }

            // Postfix increment
            Iterator operator++(int) {  // NOLINT(cert-dcl21-cpp)
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            /*
            friend long operator-(const Iterator &a, const Iterator &b) { return a.index - b.index; };

            friend Iterator operator+(const Iterator &a, int x) {
                return Iterator(a.container, a.containerDataSize, a.containerIndex, a.index + x);
            };

            friend Iterator operator-(const Iterator &a, int x) {
                return Iterator(a.container, a.containerDataSize, a.containerIndex, a.index - x);
            };
            //*/

            friend bool operator==(const Iterator &a, const Iterator &b) {
                return a.container == b.container && a.index == b.index && a.containerDataSize == b.containerDataSize &&
                       a.containerIndex == b.containerIndex;
            };

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"

            friend bool operator!=(const Iterator &a, const Iterator &b) {
                return !(a == b);
            };

#pragma clang diagnostic pop

        private:
            std::vector<value_type> *container;
            unsigned containerSize, containerDataSize, containerIndex, containerStartIndex, index;
        };

        struct ConstIterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;  // not ok...
            using value_type = T;
            using pointer = const value_type *;
            using reference = const value_type &;

            explicit ConstIterator(const std::vector<value_type> *data, unsigned dataSize, unsigned dataIndex,
                                   unsigned index) :
                    container(data), containerDataSize(dataSize), index(index), containerIndex(dataIndex),
                    containerSize(data != nullptr ? data->size() : 0), containerStartIndex() {
                this->containerStartIndex =
                        (dataIndex + this->containerSize - this->containerDataSize + this->index) % this->containerSize;
            }

            reference operator*() const { return (*this->container)[this->containerStartIndex]; }

            pointer operator->() { return &(*this); }

            // Prefix increment
            ConstIterator &operator++() {
                this->index++;
                this->containerStartIndex = (this->containerStartIndex + 1) % this->containerSize;
                return *this;
            }

            // Postfix increment
            ConstIterator operator++(int) {  // NOLINT(cert-dcl21-cpp)
                ConstIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const ConstIterator &a, const ConstIterator &b) {
                return a.container == b.container && a.index == b.index && a.containerDataSize == b.containerDataSize &&
                       a.containerIndex == b.containerIndex;
            };

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"

            friend bool operator!=(const ConstIterator &a, const ConstIterator &b) {
                return !(a == b);
            };

#pragma clang diagnostic pop

        private:
            const std::vector<value_type> *container;
            unsigned containerSize, containerDataSize, containerIndex, containerStartIndex, index;
        };

        explicit SlidingWindow(unsigned size = 0) : data(size), index(0), size(size), dataSize(0) {}

        bool empty() const {
            return this->dataSize == 0;
        }

        bool full() const {
            return this->dataSize == this->size;
        }

        unsigned getSize() const {
            return this->size;
        }

        unsigned getDataSize() const {
            return this->dataSize;
        }

        void addData(T newData) {
            assert(this->size > 0);
            this->data[this->index] = newData;
            this->index += 1;
            if (this->index == this->data.size()) {
                this->index = 0;
            }
            if (this->dataSize < this->size) {
                this->dataSize++;
            }
            // std::cout << this->index << "; " << this->dataSize << "; " << this->size << "; " << std::endl;
        }

        void addMoveData(T &newData) {
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

        const T &getLatest() const {
            return this->data[this->latestIndex()];
        }

        T &getEarliest() {
            return this->data[this->earliestIndex()];
        }

        const T &getEarliest() const {
            return this->data[this->earliestIndex()];
        }

        std::vector<T> &getData() {
            return this->data;
        }

        const std::vector<T> &getData() const {
            return this->data;
        }

        Iterator begin() { return Iterator(&this->data, this->dataSize, this->index, 0); }

        Iterator end() { return Iterator(&this->data, this->dataSize, this->index, this->dataSize); }

        ConstIterator begin() const { return ConstIterator(&this->data, this->dataSize, this->index, 0); }

        ConstIterator end() const { return ConstIterator(&this->data, this->dataSize, this->index, this->dataSize); }

        bool isWindowStable(double stabilityThreshold = 1e-9) const {
            return isSequenceStable(this->getData(), stabilityThreshold);
        }

        bool isWindowStable(std::function<double(const T &, const T &)> const &op,
                            double stabilityThreshold = 1e-9) const {
            return isSequenceStable(this->getData(), op, stabilityThreshold);
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

        void addMoveData(T &newData, bool valid = true) {
            assert(this->size > 0);
            this->validData[this->index] = valid ? 1 : 0;
            SlidingWindow<T>::addMoveData(newData);
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

        std::vector<T> &getDataIfAllValid() {
            if (!this->checkAllValid()) {
                throw std::runtime_error("Data contains invalid values!");
            }
            return this->data;
        }

        std::vector<T> const &getDataIfAllValid() const {
            if (!this->checkAllValid()) {
                throw std::runtime_error("Data contains invalid values!");
            }
            return this->data;
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

        bool isWindowStable(InvalidValuesHandlingMode invalidValuesHandlingMode,
                            double stabilityThreshold = 1e-9) const {
            return isSequenceStable(this->getDataInCorrectOrder(invalidValuesHandlingMode), stabilityThreshold);
        }

        bool isWindowStable(std::function<double(const T &, const T &)> const &op,
                            InvalidValuesHandlingMode invalidValuesHandlingMode,
                            double stabilityThreshold = 1e-9) const {
            return isSequenceStable(this->getDataInCorrectOrder(invalidValuesHandlingMode), op, stabilityThreshold);
        }

    protected:
        bool checkAllValid() {
            return std::all_of(this->validData.begin(), this->validData.end(), [](uint8_t const &x) { return x != 0; });
        }

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
