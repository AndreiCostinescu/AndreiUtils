//
// Created by Andrei on 16.01.23.
//

#include <AndreiUtils/utilsEigenOpenCV.h>
#include <AndreiUtils/classes/CrossBilateralFilter.hpp>
#include <AndreiUtils/traits/Container2DEigen.hpp>
#include <AndreiUtils/utilsOpenCV.h>
#include <AndreiUtils/utilsOpenCV.hpp>
#include <AndreiUtils/utilsTime.h>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

using namespace AndreiUtils;
using namespace cv;
using namespace Eigen;
using namespace std;

void eigenOpenCVTesting() {
    Matrix2i m = Matrix2i::Identity();
    m(0, 1) = 2;
    cv::FileStorage wfs("tmp.xml", cv::FileStorage::WRITE);
    m(1, 0) = 3;
    wfs << "matrix" << m;
    wfs.release();
    cv::FileStorage rfs("tmp.xml", cv::FileStorage::READ);
    MatrixXi m3;
    rfs["matrix"] >> m3;
    rfs.release();
    cout << "Reconstructed matrix:" << endl << m3 << endl;
}

void testCrossBilateralFilter() {
    cout << ModifiableContainer2D<int>::isContainer2D << endl;
    cout << ModifiableContainer2D<double>::isContainer2D << endl;
    cout << ModifiableContainer2D<cv::Mat>::isContainer2D << endl;
    cout << ModifiableContainer2D<Eigen::Matrix3d>::isContainer2D << endl;
    cout << ModifiableContainer2D<Eigen::MatrixXf>::isContainer2D << endl;
    cout << ModifiableContainer2D<Eigen::Vector2d>::isContainer2D << endl;
    Eigen::MatrixXd m = Eigen::MatrixXd::Identity(21, 21);
    CrossBilateralFilter filter(5);
    float x, y;
    filter.filter(11, 11, m, x, y);
    cout << "x = " << x << ", y = " << y << endl;
}

void testOpenCVMatrixAccessors() {
    auto printMatrixElements = [](std::function<float(int, int)> const &f) {
        cout << f(10, 10) << endl;
        cout << f(4, 10) << endl;
        cout << f(1, 1) << endl;
        cout << f(5, 5) << endl;
        cout << f(1, -1) << endl;
        cout << f(1, -1) << endl;
        cout << endl;
    };

    cv::Mat m1(10, 10, CV_64F);
    m1.setTo(1);
    auto getElement = getMatrixElementAccessorWithReferenceToData<double, int>(m1, 10);
    printMatrixElements(getElement);

    cv::Mat *m2 = &m1;
    m1.setTo(4);
    cout << "m2 = " << m2 << endl;
    auto getElementPtr = getMatrixElementAccessorWithReferenceToData<double, int>(m2, 14);
    printMatrixElements(getElementPtr);

    auto *m3 = new cv::Mat(10, 10, CV_64F);
    m3->setTo(64);
    cout << "m3 = " << m3 << endl;
    auto getElementPtr2 = getMatrixElementAccessorWithReferenceToData<double, int>(m3, 19);
    printMatrixElements(getElementPtr2);
    delete m3;

    const auto *m4 = new cv::Mat(10, 10, CV_64F);
    cout << "m4 = " << m4 << endl;
    auto getElementPtr3 = getMatrixElementAccessorWithReferenceToData<double, int>(m4, 69);
    printMatrixElements(getElementPtr3);
    cout << endl;

    auto const *const &m5 = m4;
    cout << "m5 = " << m5 << endl;
    auto getElementPtr4 = getMatrixElementAccessorWithReferenceToData<double, float>(m5, 2.4);
    printMatrixElements(getElementPtr4);
    delete m4;

    function<float(int, int)> getElementPtr5;
    {
        void *m6 = m2;
        getElementPtr5 = getMatrixElementAccessorWithReferenceToData<double, float>((cv::Mat *) m6, 2.4999);
    }
    printMatrixElements(getElementPtr5);

    function<float(int, int)> getElementPtr6;
    {
        void *m6 = m2;
        getElementPtr6 = getMatrixElementAccessor<double, float>((cv::Mat *) m6, 2.4999);
    }
    printMatrixElements(getElementPtr6);

    function<float(int, int)> getElementPtr7;
    void *m7 = new cv::Mat(10, 10, CV_64F);
    ((cv::Mat *) m7)->setTo(69);
    {
        getElementPtr7 = getMatrixElementAccessor<double, float>(*(cv::Mat *) m7, 2.7);
    }
    delete (cv::Mat *) m7;
    m7 = m2;
    printMatrixElements(getElementPtr7);

    function<float(int, int)> getElementPtr8;
    m7 = new cv::Mat(10, 10, CV_64F);
    ((cv::Mat *) m7)->setTo(48);
    {
        getElementPtr8 = getMatrixElementAccessorWithReferenceToData<double, float>(*(cv::Mat *) m7, 2.8);
    }
    delete (cv::Mat *) m7;
    m7 = m2;
    printMatrixElements(getElementPtr8);
}

void testPrintingImagesOpenCV() {
    cv::Mat m = cv::Mat(300, 300, CV_8U);
    m.setTo(100);
    displayImage(m, "GrayScale");
    for (int i = 0; i < 4; i++) {
        for (int v = 0; v < 256; v++) {
            m.setTo(v);
            displayImage(m, "GrayScale");
            cv::waitKey(2);
        }
        for (int v = 255; v >= 0; v--) {
            m.setTo(v);
            displayImage(m, "GrayScale");
            cv::waitKey(2);
        }
    }
    cout << "Finished visualization!" << endl;
    cv::waitKey();
    cout << "END PROGRAM!" << endl;
}

void testOpenCVMatrixCropReference() {
    cv::Mat m = cv::Mat(300, 300, CV_8U);
    m.setTo(100);

    for (int i = 0; i < 4; i++) {
        for (int v = 0; v < 256; v++) {
            cv::Mat view = m(cv::Range(100, 200), cv::Range(100, 200));
            view.setTo(v);
            displayImage(m, "GrayScale");
            cv::waitKey(10);
        }
        for (int v = 255; v >= 0; v--) {
            m.setTo(v);
            displayImage(m, "GrayScale");
            cv::waitKey(10);
        }
    }
}

void testMatrixRelease() {
    cv::Mat x = Mat(10, 10, CV_64F);
    cout << x.empty() << endl;
    cout << x.size << endl;
    x.release();
    cout << x.empty() << endl;
}

void testAddColorBorder() {
    cv::Mat img(100, 100, CV_8UC3, cv::Scalar(255, 0, 0));
    cv::imshow("Output", addColorBorderAroundImage(img, cv::Scalar(0, 0, 255), 10));
    cv::waitKey(0);
}

int main() {
    cout << "Hello World!" << endl;

    // eigenOpenCVTesting();
    // testCrossBilateralFilter();
    // testOpenCVMatrixAccessors();
    // testPrintingImagesOpenCV();
    // testOpenCVMatrixCropReference();
    // testMatrixRelease();
    testAddColorBorder();

    return 0;
}