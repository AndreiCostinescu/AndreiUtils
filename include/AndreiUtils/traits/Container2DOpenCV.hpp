//
// Created by Andrei on 20-Mar-22.
//

#ifndef ANDREIUTILS_CONTAINER2DOPENCV_HPP
#define ANDREIUTILS_CONTAINER2DOPENCV_HPP

#include <AndreiUtils/traits/Container2D.hpp>
#include <opencv2/opencv.hpp>

namespace AndreiUtils {
    template<typename Scalar>
    class Container2D<cv::Mat> {
    public:
        static const bool isContainer2D = true;

        Container2D(const cv::Mat &data) : data(data) {}

        inline int height() const {
            return this->data.rows;
        }

        inline int width() const {
            return this->data.cols;
        }

        Scalar valueAt(int row, int col) const {
            return this->data.at<Scalar>(row, col);
        }

        bool checkPointInsideContainer(int row, int col) const {
            return 0 <= row && row < this->height() && 0 <= col && col < this->width();
        }

        bool checkPointInsideContainer(double row, double col) const {
            return 0 <= row && row < this->height() && 0 <= col && col < this->width();
        }

    protected:
        const cv::Mat &data;
    };

    template<typename Scalar>
    class ModifiableContainer2D<cv::Mat> : public Container2D<cv::Mat> {
    public:
        ModifiableContainer2D(cv::Mat &data) : Container2D(data), data(data) {}

        void setValueAt(int row, int col, const Scalar &value) {
            this->modifiabledata.at<Scalar>(row, col) = value;
        }

    protected:
        const cv::Mat &data;
    };
}

#endif //ANDREIUTILS_CONTAINER2DOPENCV_HPP
