//
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/classes/trajectory/JointTrajectory.hpp>
#include <AndreiUtils/classes/SlidingWindow.hpp>
#include <AndreiUtils/traits/median_computer_eigen.hpp>
#include <AndreiUtils/utilsEigen.hpp>
#include <AndreiUtils/utilsEigenGeometry.h>
#include <AndreiUtils/utilsEigenGeometry.hpp>
#include <AndreiUtils/classes/SlerpInterpolator.hpp>
#include <iostream>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

void testJointTrajectoryClass() {
    JointTrajectory<7> jointTrajectory(10);
    cout << jointTrajectory.getSize() << endl;
    for (auto const &jointValues: jointTrajectory.getJointValues()) {
        cout << jointValues.transpose() << endl;
    }
}

void testEigenSlidingWindow() {
    SlidingWindow<Eigen::Vector3d> swEigen(11);
    for (int i = 0; i < 20; i++) {
        swEigen.addData(Vector3d((double) i, (double) (i * i), (double) (i * i * i)));
        cout << "At i = " << i << ": median = " << swEigen.getMedian() << ", average = " << swEigen.getAverage()
             << endl;
    }
}

void testEigenMatrixAddSub() {
    Eigen::Matrix3d x;
    x.setZero();
    cout << addComponentWise<double>(x, -1) << endl;
    Eigen::Vector4f y;
    y.setZero();
    cout << addComponentWise<float>(y, 4) << endl;

    for (int i = 0; i < 10; i++) {
        cout << sampleOrientation() << endl;
        cout << sampleDirection().transpose() << endl;
    }
}

void testSurfaceTriangle() {
    checkInsideTriangles<double>(Vector3d(0, 0, 1), Matrix<double, 3, 4>(), Matrix<double, 4, 3>(), 4, 4);
}

void testInterpolation() {
    SlerpInterpolator<double> sx;
    sx.compute(Quaterniond(1, 0, 0, 0), Quaterniond(0, 0, 0, 1), 101, true, true);
    printVector(sx.getResult());
    sx.compute(Quaterniond(1, 0, 0, 0), Quaterniond(0, 0, 0, 1), 100, true, false);
    printVector(sx.getResult());
    sx.compute(Quaterniond(1, 0, 0, 0), Quaterniond(0, 0, 0, 1), 100, false, true);
    printVector(sx.getResult());
    sx.compute(Quaterniond(1, 0, 0, 0), Quaterniond(0, 0, 0, 1), 99, false, false);
    printVector(sx.getResult());
}

void testAnyOrientationFromAxis() {
    Vector3d axis(0, 0, 1);
    cout << getAnyOrientationFromOneAxis(axis, "x") << endl;
    cout << getAnyOrientationFromOneAxis(axis, "y") << endl;
    cout << getAnyOrientationFromOneAxis(axis, "z") << endl;
    cout << getAnyOrientationQuaternionFromOneAxis(axis, "x") << endl;
    cout << getAnyOrientationQuaternionFromOneAxis(axis, "y") << endl;
    cout << getAnyOrientationQuaternionFromOneAxis(axis, "z") << endl;
    axis << 1, 0, 0;
    cout << getAnyOrientationFromOneAxis(axis, "x") << endl;
    cout << getAnyOrientationFromOneAxis(axis, "y") << endl;
    cout << getAnyOrientationFromOneAxis(axis, "z") << endl;
    cout << getAnyOrientationQuaternionFromOneAxis(axis, "x") << endl;
    cout << getAnyOrientationQuaternionFromOneAxis(axis, "y") << endl;
    cout << getAnyOrientationQuaternionFromOneAxis(axis, "z") << endl;
}

void testSampleVectors() {
    int N = 100;
    for (int i = 0; i < N; ++i) {
        cout << sampleVectorNormal<double, 3>(Vector3d::Zero(), 1).transpose() << endl;
    }
    cout << endl;
    for (int i = 0; i < N; ++i) {
        cout << sampleVectorNormal<double, 3>(Vector3d::Zero(), Vector3d{1.0, 0.5, 0.75}) << endl;
    }
}

int main() {
    cout << "Hello World!" << endl;

    // testJointTrajectoryClass();
    // testEigenSlidingWindow();
    // testEigenMatrixAddSub();
    // testSurfaceTriangle();
    // testInterpolation();
    // testAnyOrientationFromAxis();
    testSampleVectors();

    return 0;
}
