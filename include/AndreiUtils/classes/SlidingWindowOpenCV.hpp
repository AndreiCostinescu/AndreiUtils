//
// Created by andrei on 03.12.20.
//

#ifndef ANDREIUTILS_SLIDINGWINDOWOPENCV_HPP
#define ANDREIUTILS_SLIDINGWINDOWOPENCV_HPP

#include <AndreiUtils/classes/SlidingWindow.hpp>

namespace AndreiUtils {

    template<>
    class SlidingWindow<cv::Point3f> {
    public:
        explicit SlidingWindow(unsigned size = 0) : data(size), index(0), size(size), dataSize(0) {}

        void addData(const cv::Point3f &newData) {
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

        void clear() {
            this->index = 0;
            this->dataSize = 0;
        }

        cv::Point3f convolve(const std::vector<double> &parameters) const {
            bool average = false;
            if (parameters.size() != this->dataSize) {
                average = true;
            }
            cv::Point3f res;
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

        cv::Point3f getMedian() const {
            std::vector<cv::Point3f> values = this->getDataInCorrectOrder();
            std::vector<float> dimensionValues(values.size());
            cv::Point3f res;
            double value;
            for (int dim = 0; dim < 3; dim++) {
                for (int i = 0; i < values.size(); i++) {
                    if (dim == 0) {
                        value = values[i].x;
                    } else if (dim == 1) {
                        value = values[i].y;
                    } else if (dim == 2) {
                        value = values[i].z;
                    }
                    dimensionValues[i] = value;
                }
                if (dim == 0) {
                    res.x = AndreiUtils::median(dimensionValues);
                } else if (dim == 1) {
                    res.y = AndreiUtils::median(dimensionValues);
                } else if (dim == 2) {
                    res.z = AndreiUtils::median(dimensionValues);
                }
            }
            return res;
        }

        cv::Point3f getAverage() const {
            return AndreiUtils::average(this->getDataInCorrectOrder());
        }

        std::vector<cv::Point3f> &getData() {
            return this->data;
        }

    private:
        std::vector<cv::Point3f> getDataInCorrectOrder() const {
            std::vector<cv::Point3f> a(this->dataSize);
            for (unsigned int i = this->dataSize; i >= 1; i--) {
                a[this->dataSize - i] = this->data[(this->index + this->size - i) % this->size];
            }
            return a;
        }

        std::vector<cv::Point3f> data;
        unsigned index, size, dataSize;
    };
}

#endif //ANDREIUTILS_SLIDINGWINDOWOPENCV_HPP
