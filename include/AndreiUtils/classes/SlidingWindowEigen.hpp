//
// Created by andrei on 03.12.20.
//

#ifndef ANDREIUTILS_SLIDINGWINDOWEIGEN_HPP
#define ANDREIUTILS_SLIDINGWINDOWEIGEN_HPP

#include <AndreiUtils/classes/SlidingWindow.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    class SlidingWindow<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
    public:
        explicit SlidingWindow(unsigned size = 0) : data(size), index(0), size(size), dataSize(0) {}

        void addData(Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> newData) {
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

        Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> convolve(
                const std::vector<Scalar> &parameters) const {
            bool average = false;
            if (parameters.size() != this->dataSize) {
                average = true;
            }
            Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> res;
            Scalar parameter;
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

        Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> getMedian() const {
            std::vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> values = this->getDataInCorrectOrder();
            std::vector<Scalar> dimensionValues(values.size());
            Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> res;
            if (values.empty()) {
                return res;
            }
            int nrDimensions = values[0].size();
            res = Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>::Zero(nrDimensions);
            for (int dim = 0; dim < nrDimensions; dim++) {
                for (int i = 0; i < values.size(); i++) {
                    dimensionValues[i] = values[i][dim];
                }
                res[dim] = AndreiUtils::median(dimensionValues);
            }
            return res;
        }

        Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> getAverage() const {
            return AndreiUtils::average(this->getDataInCorrectOrder());
        }

        std::vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> &getData() {
            return this->data;
        }

    protected:
        std::vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> getDataInCorrectOrder() const {
            std::vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> a(this->dataSize);
            for (unsigned int i = this->dataSize; i >= 1; i--) {
                a[this->dataSize - i] = this->data[(this->index + this->size - i) % this->size];
            }
            return a;
        }

        std::vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> data;
        unsigned index, size, dataSize;
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    class SlidingWindowWithInvalidValues<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> :
            public SlidingWindow<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
    public:
        explicit SlidingWindowWithInvalidValues(unsigned size = 0)
                : SlidingWindow<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>>(size), validData(size) {}

        void addData(Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> newData, bool valid = true) {
            assert(this->size > 0);
            this->validData[this->index] = valid;
            SlidingWindow<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>>::addData(newData);
        }

        Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>
        convolve(const std::vector<double> &parameters, InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            bool average = false;
            if (parameters.size() != this->dataSize) {
                average = true;
            }
            Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> res;
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

        Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>
        getMedian(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            return AndreiUtils::median(this->getDataInCorrectOrder(invalidValuesHandlingMode));
        }

        Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>
        getAverage(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            return AndreiUtils::average(this->getDataInCorrectOrder(invalidValuesHandlingMode));
        }

        std::vector<bool> &getValidFlags() {
            return this->validData;
        }

    protected:
        std::vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>>
        getDataInCorrectOrder(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            std::vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> a(this->dataSize);
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

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    class SlidingWindow<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
    public:
        explicit SlidingWindow(unsigned size = 0) : data(size), index(0), size(size), dataSize(0) {}

        void addData(Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> newData) {
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

        Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> convolve(
                const std::vector<Scalar> &parameters) const {
            bool average = false;
            if (parameters.size() != this->dataSize) {
                average = true;
            }
            Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> res;
            Scalar parameter;
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

        Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> getMedian() const {
            std::vector<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> values = this->getDataInCorrectOrder();
            std::vector<Scalar> dimensionValues(values.size());
            Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> res;
            if (values.empty()) {
                return res;
            }
            int nrDimensions = values[0].size();
            res = Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>::Zero(nrDimensions);
            for (int dim = 0; dim < nrDimensions; dim++) {
                for (int i = 0; i < values.size(); i++) {
                    dimensionValues[i] = values[i][dim];
                }
                res[dim] = AndreiUtils::median(dimensionValues);
            }
            return res;
        }

        Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> getAverage() const {
            return AndreiUtils::average(this->getDataInCorrectOrder());
        }

        std::vector<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> &getData() {
            return this->data;
        }

    protected:
        std::vector<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> getDataInCorrectOrder() const {
            std::vector<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> a(this->dataSize);
            for (unsigned int i = this->dataSize; i >= 1; i--) {
                a[this->dataSize - i] = this->data[(this->index + this->size - i) % this->size];
            }
            return a;
        }

        std::vector<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> data;
        unsigned index, size, dataSize;
    };

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    class SlidingWindowWithInvalidValues<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> :
            public SlidingWindow<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
    public:
        explicit SlidingWindowWithInvalidValues(unsigned size = 0)
                : SlidingWindow<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>>(size), validData(size) {}

        void addData(Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> newData, bool valid = true) {
            assert(this->size > 0);
            this->validData[this->index] = valid;
            SlidingWindow<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>>::addData(newData);
        }

        Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>
        convolve(const std::vector<double> &parameters, InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            bool average = false;
            if (parameters.size() != this->dataSize) {
                average = true;
            }
            Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols> res;
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

        Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>
        getMedian(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            return AndreiUtils::median(this->getDataInCorrectOrder(invalidValuesHandlingMode));
        }

        Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>
        getAverage(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            return AndreiUtils::average(this->getDataInCorrectOrder(invalidValuesHandlingMode));
        }

        std::vector<bool> &getValidFlags() {
            return this->validData;
        }

    protected:
        std::vector<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>>
        getDataInCorrectOrder(InvalidValuesHandlingMode invalidValuesHandlingMode) const {
            std::vector<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> a(this->dataSize);
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

#endif //ANDREIUTILS_SLIDINGWINDOWEIGEN_HPP
