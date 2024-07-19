//
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/utilsJsonEigen.hpp>
#include <iostream>
#include <gtest/gtest.h>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

void eigenJsonTesting() {
    Matrix2i m = Matrix2i::Identity();
    m(0, 1) = 2;
    nlohmann::json j = m;
    cout << "Json from matrix m " << endl << m << endl << j << endl;
    Matrix2i m2 = j.get<Matrix2i>();
    cout << "Reconstructed matrix:" << endl << m2 << endl;
    MatrixXi m3 = j.get<MatrixXi>();
    cout << "Reconstructed matrix:" << endl << m3 << endl;

    Vector3f v(5, 3.6, 21);
    j = v;
    cout << "Json from vector v " << endl << v << endl << j << endl;
    Vector3f v2 = j.get<Vector3f>();
    cout << "Reconstructed vector:" << endl << v2 << endl;
}

void testEigenFromVectorDeserialization() {
    nlohmann::json j;
    j = vector<double>{0, 0.5, 1, 1.5, 2, 2.5};

    auto x1 = j.get<Eigen::Matrix<double, 2, 3>>();
    cout << x1 << endl << endl;

    auto x2 = j.get<Eigen::Matrix<double, 3, 2>>();
    cout << x2 << endl << endl;

    auto x3 = j.get<Eigen::Matrix<int, 2, 3>>();
    cout << x3 << endl << endl;

    auto x4 = j.get<Eigen::Matrix<int, 3, 2>>();
    cout << x4 << endl << endl;

    auto x5 = j.get<Eigen::MatrixXd>();
    cout << x5 << endl << endl;

    auto x6 = j.get<Eigen::VectorXd>();
    cout << x6 << endl << endl;

    auto x7 = j.get<Eigen::RowVectorXd>();
    cout << x7 << endl << endl;

    auto x8 = j.get<Eigen::Matrix<double, -1, 3>>();
    cout << x8 << endl << endl;

    auto x9 = j.get<Eigen::Matrix<double, 3, -1>>();
    cout << x9 << endl << endl;

    try {
        j.get<Eigen::Matrix<double, 4, -1>>();
        assert(false);
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    try {
        j.get<Eigen::Matrix<double, 1, 2>>();
        assert(false);
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    try {
        j.get<Eigen::Matrix<double, -1, 5>>();
        assert(false);
    } catch (exception &e) {
        cout << e.what() << endl;
    }
}

TEST(EigenJsonTest, eigenJson) {
    Matrix2i m = Matrix2i::Identity();
    m(0, 1) = 2;
    nlohmann::json j = m;

    Matrix2i m2;
    EXPECT_NO_THROW(m2 = j.get<Matrix2i>());
    EXPECT_EQ(m, m2) << "Matrix2i reconstruction failed";

    MatrixXi m3;
    EXPECT_NO_THROW(m3 = j.get<MatrixXi>());
    EXPECT_EQ(m(0, 0), m3(0, 0)) << "MatrixXi reconstruction failed at (0,0)";
    EXPECT_EQ(m(0, 1), m3(0, 1)) << "MatrixXi reconstruction failed at (0,1)";
    EXPECT_EQ(m(1, 0), m3(1, 0)) << "MatrixXi reconstruction failed at (1,0)";
    EXPECT_EQ(m(1, 1), m3(1, 1)) << "MatrixXi reconstruction failed at (1,1)";

    ASSERT_EQ(m, m3);

    Vector3f v(5, 3.6, 21);
    j = v;

    Vector3f v2;
    EXPECT_NO_THROW(v2 = j.get<Vector3f>());
    EXPECT_EQ(v, v2) << "Vector3f reconstruction failed";
}

TEST(EigenJsonTest, testEigenFromVectorDeserialization) {
    nlohmann::json j;
    j = vector<double>{0, 0.5, 1, 1.5, 2, 2.5};

    auto x1 = j.get<Eigen::Matrix<double, 2, 3>>();
    Eigen::Matrix<double, 2, 3> expected1;
    expected1 << 0, 0.5, 1, 1.5, 2, 2.5;
    EXPECT_EQ(x1, expected1);

    auto x2 = j.get<Eigen::Matrix<double, 3, 2>>();
    Eigen::Matrix<double, 3, 2> expected2;
    expected2 << 0, 0.5, 1 ,1.5, 2, 2.5;
    EXPECT_EQ(x2, expected2);

    auto x3 = j.get<Eigen::Matrix<int, 2, 3>>();
    Eigen::Matrix<int, 2, 3> expected3;
    expected3 << 0, 0, 1, 1, 2, 2;
    EXPECT_EQ(x3, expected3);

    auto x4 = j.get<Eigen::Matrix<int, 3, 2>>();
    Eigen::Matrix<int, 3, 2> expected4;
    expected4 << 0,0, 1, 1, 2, 2;
    EXPECT_EQ(x4, expected4);

    auto x5 = j.get<Eigen::MatrixXd>();
    Eigen::MatrixXd expected5(6, 1);
    expected5 << 0, 0.5, 1, 1.5, 2, 2.5;
    EXPECT_EQ(x5, expected5);

    auto x6 = j.get<Eigen::VectorXd>();
    Eigen::VectorXd expected6(6);
    expected6 << 0, 0.5, 1, 1.5, 2, 2.5;
    EXPECT_EQ(x6, expected6);

    auto x7 = j.get<Eigen::RowVectorXd>();
    Eigen::RowVectorXd expected7(6);
    expected7 << 0, 0.5, 1, 1.5, 2, 2.5;
    EXPECT_EQ(x7, expected7);

    auto x8 = j.get<Eigen::Matrix<double, -1, 3>>();
    Eigen::Matrix<double, 2, 3> expected8;
    expected8 << 0, 1, 2,0.5 ,1.5 ,2.5;
    EXPECT_EQ(x8, expected8);

    auto x9 = j.get<Eigen::Matrix<double, 3, -1>>();
    Eigen::Matrix<double, 3, 2> expected9;
    expected9 << 0, 1.5, 0.5, 2, 1, 2.5;
    EXPECT_EQ(x9, expected9);

    EXPECT_THROW((j.get<Eigen::Matrix<double, 4, -1>>()), std::exception);
    EXPECT_THROW((j.get<Eigen::Matrix<double, 1, 2>>()), std::exception);
    EXPECT_THROW((j.get<Eigen::Matrix<double, -1, 5>>()), std::exception);
}


int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    // eigenJsonTesting();
    // testEigenFromVectorDeserialization();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


