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
#include <gtest/gtest.h>

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

TEST(EigenTest, JointTrajectory) {
    JointTrajectory<7> jointTrajectory(10);
    EXPECT_EQ(jointTrajectory.getSize(), 10);

    const auto& jointValues = jointTrajectory.getJointValues();
    for (const auto& jointValue : jointValues) {
        EXPECT_EQ(jointValue.size(), 7);
        Eigen::VectorXd expectedJointValue = Eigen::VectorXd::Zero(7);
        EXPECT_EQ(jointValue, expectedJointValue);
    }
}

TEST(EigenTest, EigenSlidingWindow) {
    SlidingWindow<Eigen::Vector3d> swEigen(11);

    std::vector<Eigen::Vector3d> expectedMedians = {
        {0, 0, 0}, {0.5, 0.5, 0.5}, {1, 1, 1}, {1.5, 2.5, 4.5}, {2, 4, 8}, {2.5, 6.5, 17.5},
        {3, 9, 27}, {3.5, 12.5, 45.5}, {4, 16, 64}, {4.5, 20.5, 94.5}, {5, 25, 125},
        {6, 36, 216}, {7, 49, 343}, {8, 64, 512}, {9, 81, 729}, {10, 100, 1000},
        {11, 121, 1331}, {12, 144, 1728}, {13, 169, 2197}, {14, 196, 2744}
    };

    std::vector<Eigen::Vector3d> expectedAverages = {
        {0, 0, 0}, {0.5, 0.5, 0.5}, {1, 1.66667, 3}, {1.5, 3.5, 9}, {2, 6, 20}, {2.5, 9.16667, 37.5},
        {3, 13, 63}, {3.5, 17.5, 98}, {4, 22.6667, 144}, {4.5, 28.5, 202.5}, {5, 35, 275},
        {6, 46, 396}, {7, 59, 553}, {8, 74, 752}, {9, 91, 999}, {10, 110, 1300},
        {11, 131, 1661}, {12, 154, 2088}, {13, 179, 2587}, {14, 206, 3164}
    };

    for (int i = 0; i < 20; i++) {
        swEigen.addData(Eigen::Vector3d(static_cast<double>(i), static_cast<double>(i * i), static_cast<double>(i * i * i)));

        Eigen::Vector3d median = swEigen.getMedian();
        Eigen::Vector3d average = swEigen.getAverage();

        EXPECT_NEAR(median.x(), expectedMedians[i].x(), 1e-3);
        EXPECT_NEAR(median.y(), expectedMedians[i].y(), 1e-3);
        EXPECT_NEAR(median.z(), expectedMedians[i].z(), 1e-3);

        EXPECT_NEAR(average.x(), expectedAverages[i].x(), 1e-3);
        EXPECT_NEAR(average.y(), expectedAverages[i].y(), 1e-3);
        EXPECT_NEAR(average.z(), expectedAverages[i].z(), 1e-3);
    }
    }

TEST(EigenTest, MatrixAddSub) {
    Eigen::Matrix3d x;
    x.setZero();
    Eigen::Matrix3d resultMatrix = addComponentWise<double>(x, -1);

    Eigen::Matrix3d expectedMatrix;
    expectedMatrix.setConstant(-1);

    EXPECT_EQ(resultMatrix, expectedMatrix);

    Eigen::Vector4f y;
    y.setZero();
    Eigen::Vector4f resultVector = addComponentWise<float>(y, 4);

    Eigen::Vector4f expectedVector;
    expectedVector.setConstant(4);

    EXPECT_EQ(resultVector, expectedVector);

    for (int i = 0; i < 10; i++) {
        auto orientation = sampleOrientation();
        auto direction = sampleDirection();

        std::cout << "Sampled Orientation:\n" << orientation << std::endl;
        std::cout << "Sampled Direction:\n" << direction.transpose() << std::endl;

    }
}

TEST(EigenTest, TestTriangles) {

    Vector3d point(0, 0, 1);
    Matrix<double, 3, 4> triangles;
    triangles.setZero();
    Matrix<double, 4, 3> otherData;
    otherData.setZero();
    int rows = 4;
    int cols = 4;

    bool result = checkInsideTriangles<double>(point, triangles, otherData, rows, cols);

    EXPECT_TRUE(result);
}

TEST(EigenTest, OrientationFromAxis) {
    Vector3d axis1(0, 0, 1);

     Matrix3d result;

    result = getAnyOrientationFromOneAxis(axis1, "x");
    Matrix3d expected1;
    expected1 << 0, 0, -1,
                 0, 1,  0,
                 1, 0,  0;
   ASSERT_EQ(result, expected1);

    result = getAnyOrientationFromOneAxis(axis1, "y");
    Matrix3d expected2;
    expected2 << -1, 0, 0,
                 0, 0, 1,
                0, 1, 0;
    ASSERT_EQ(result, expected2);

    result = getAnyOrientationFromOneAxis(axis1, "z");
    Matrix3d expected3;
    expected3 << 0, -1, 0,
                 1, 0, 0,
                 0, 0, 1;
    ASSERT_EQ(result, expected3);


    Vector3d axis2(1, 0, 0);

    result = getAnyOrientationFromOneAxis(axis2, "x");
    Matrix3d expected4;
    expected4 << 1, 0, 0,
                 0, 0, -1,
                 0, 1, 0;
    ASSERT_EQ(result, expected4);

    result = getAnyOrientationFromOneAxis(axis2, "y");
    Matrix3d expected5;
    expected5 << 0, 1, 0,
                 -1, 0, 0,
                0, 0, 1;
    ASSERT_EQ(result, expected5);

    result = getAnyOrientationFromOneAxis(axis2, "z");
    Matrix3d expected6;
    expected6 << 0, 0, 1,
                 0, -1, 0,
                 1, 0, 0;
    ASSERT_EQ(result, expected6);
}

int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    //testJointTrajectoryClass();
    // testEigenSlidingWindow();
    // testEigenMatrixAddSub();
    // testSurfaceTriangle();
    // testInterpolation(); ?
    // testAnyOrientationFromAxis(); ?

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
