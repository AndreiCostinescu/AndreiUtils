//
// Created by andrei on 03.12.20.
//

#ifndef ANDREIUTILS_SLIDINGWINDOWOPENCV_HPP
#define ANDREIUTILS_SLIDINGWINDOWOPENCV_HPP

#include <AndreiUtils/classes/SlidingWindow.hpp>
#include <AndreiUtils/enums/InvalidValuesHandlingMode.h>
#include <opencv2/opencv.hpp>

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

    protected:
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

    template<>
    class SlidingWindowWithInvalidValues<cv::Point3f> : public SlidingWindow<cv::Point3f> {
    public:
        explicit SlidingWindow(unsigned size = 0) : SlidingWindow<cv::Point3f>(size), validData(size) {}

        void addData(const cv::Point3f &newData, bool valid = true) {
            assert(this->size > 0);
            this->validData[this->index] = valid;
            SlidingWindow < cv::Point3f > ::addData(newData);
        }

        cv::Point3f convolve(const std::vector<double> &parameters) const {
            bool average = false;
            if (parameters.size() != this->dataSize) {
                average = true;
            }
            cv::Point3f res;
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

        cv::Point3f getMedian(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            std::vector<cv::Point3f> values = this->getDataInCorrectOrder(invalidValuesHandlingMode);
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

        cv::Point3f getAverage(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            return AndreiUtils::average(this->getDataInCorrectOrder(invalidValuesHandlingMode));
        }

        std::vector<bool> &getValidFlags() {
            return this->validData;
        }

    protected:
        std::vector<cv::Point3f> getDataInCorrectOrder(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            std::vector<cv::Point3f> a(this->dataSize);
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

#endif //ANDREIUTILS_SLIDINGWINDOWOPENCV_HPP
