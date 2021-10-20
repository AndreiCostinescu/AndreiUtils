//
// Created by andrei on 03.12.20.
//

#ifndef ANDREIUTILS_SLIDINGWINDOW_HPP
#define ANDREIUTILS_SLIDINGWINDOW_HPP

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

    private:
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
}

#endif //ANDREIUTILS_SLIDINGWINDOW_HPP
