//
// Created by Andrei on 03.11.22.
//

#include <algorithm>
#include <AndreiUtils/utilsVector.hpp>
#include <Eigen/Dense>
#include <random>
#include <gtest/gtest.h>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

void testSortMultipleVectorsBasedOnOneCriterion() {
    vector<double> x(10);
    std::iota(x.begin(), x.end(), 10);
    vector<int> y(10);
    std::iota(y.begin(), y.end(), 4);
    shuffle(y.begin(), y.end(), std::mt19937(std::random_device()()));
    vector<string> z(10);
    for (int i = 0; i < z.size(); i++) {
        if (i == 0) {
            z[i] = "1";
        } else {
            z[i] = z[i - 1] + "1";
        }
    }
    auto permutation = getSortedIndicesOfVector(y, [](const int &a, const int &b) {
        return a < b;
    });
    printVector(x);
    printVector(y);
    printVector(z);
    printVector(permutation);
    sortMultipleVectorsBasedOnPermutation(permutation, x, y, z);
    printVector(x);
    printVector(y);
    printVector(z);
}

void testVectorAppendFunctions() {
    Eigen::Vector3d x(1, 2, 3);
    vector<double> y{1, 2, 3, 4, 5, 6, 7};
    printVector(x.data(), 3);
    printVector(y);
    vector<double> res, res2;
    vectorAppendInPlace(res, y);
    printVector(res);
    vectorAppendInPlace(res, x.data(), 3);
    printVector(res);
    res2 = vectorAppend(y, x.data(), 3);
    printVector(res2);
    res2 = vectorAppend(res, x.data(), 3);
    printVector(res2);
    res2 = vectorAppend(res, y);
    printVector(res2);
    cout << "Done" << endl;
}

void testVectorEquals() {
    Eigen::Vector3d x(1, 2, 3);
    vector<double> y{1, 2, 3, 4, 5, 6, 7};
    vector<double> res, res2;
    vectorAppendInPlace(res, y);
    cout << vectorEquals(y, res) << endl;
    vectorAppendInPlace(res, x.data(), 3);
    cout << vectorEquals(y, res) << endl;
}

void testSpliceVector() {
    int n = 10;
    vector<double> x(n);
    for (int i = 0; i < n; i++) {
        x[i] = i + 1;
    }
    printVector(spliceVector(x, 0, 9));
    printVector(spliceVector(x, 1, 8));
    printVector(spliceVector(x, 6, 4));
    printVector(spliceVector(x, 0, -1));
    printVector(spliceVector(x, 10, 9));
}

void testRemoveDuplicates() {
    vector<int> x{10, 10, 10, 10, 1, 12, 13, 14, 1, 1, 1};
    set<int> s(x.begin(), x.end());
    for (auto &data: s) {
        cout << data << endl;
    }
    vector<int> v(s.begin(), s.end());
    printVector(v);
    printVector(x);
    printVector(removeDuplicates(x));
}

void testEigenDataAsArray() {
    Matrix<double, 2, 3> bb;
    bb << -0.113, -0.113, -0.003, 0.113, 0.113, 0.016;
    cout << bb << endl;
    cout << bb(0) << endl;
    cout << bb(1) << endl;
    cout << bb(2) << endl;
    cout << bb(3) << endl;
    cout << bb(4) << endl;
    cout << bb(5) << endl;
    cout << bb(0, 0) << endl;
    cout << bb(0, 1) << endl;
    cout << bb(0, 2) << endl;
    cout << bb(1, 0) << endl;
    cout << bb(1, 1) << endl;
    cout << bb(1, 2) << endl;
    cout << printVectorToString(vectorFromArray(bb.transpose().data(), 6)) << endl;
}

void testResizeAndReserve() {
    vector<int> v(10);
    for (int i = 0; i < 10; ++i) {
        v[i] = i + 1;
    }
    cout << "v.size = " << v.size() << endl;
    v.reserve(20);
    cout << "v.size = " << v.size() << endl;
    for (auto const &i: v) {
        cout << i << endl;
    }
    v.emplace_back(11);
    cout << "v.size = " << v.size() << endl;
    for (auto const &i: v) {
        cout << i << endl;
    }
}

class A {
public:
    static int count;

    A() {
        cout << "Create new A: " << ++count << endl;
    }

    ~A() {
        cout << "Delete old A: " << --count << endl;
    }
};

int A::count = 0;

void testCreationOfDynamicSizeArray() {
    A **m = new2dArray<A>(3, 5);
    delete2dArray(m, 3);
    assert(A::count == 0);
}


TEST(VectorTest, SortBasedOnPermutation) {
    std::vector<double> x(10);
    std::iota(x.begin(), x.end(), 10.0);

    std::vector<int> y(10);
    std::iota(y.begin(), y.end(), 4);
    std::shuffle(y.begin(), y.end(), std::mt19937(std::random_device()()));

    std::vector<std::string> z(10);
    for (int i = 0; i < z.size(); ++i) {
        if (i == 0) {
            z[i] = "1";
        } else {
            z[i] = z[i - 1] + "1";
        }
    }

    auto permutation = getSortedIndicesOfVector(y, [](const int &a, const int &b) {
        return a < b;
    });

    for (size_t i = 1; i < permutation.size(); ++i) {
        ASSERT_LE(y[permutation[i - 1]], y[permutation[i]]);
    }

    sortMultipleVectorsBasedOnPermutation(permutation, x, y, z);

    for (size_t i = 1; i < permutation.size(); ++i) {
        ASSERT_LE(y[i - 1], y[i]);
    }

}

TEST(VectorTest, AppendFunctions) {

    Eigen::Vector3d x(1, 2, 3);
    std::vector<double> y{1, 2, 3, 4, 5, 6, 7};

    std::vector<double> expectedRes1 = {1, 2, 3, 4, 5, 6, 7};
    std::vector<double> expectedRes2 = {1, 2, 3, 4, 5, 6, 7, 1, 2, 3};
    std::vector<double> expectedRes3 = {1, 2, 3, 4, 5, 6, 7, 1, 2, 3};
    std::vector<double> expectedRes4 = {1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 1, 2, 3};
    std::vector<double> expectedRes5 = {1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 1, 2, 3, 4, 5, 6, 7};

    std::vector<double> res;

    vectorAppendInPlace(res, y);
    ASSERT_EQ(res, expectedRes1);

    vectorAppendInPlace(res, x.data(), 3);
    ASSERT_EQ(res, expectedRes2);

    std::vector<double> res2 = vectorAppend(y, x.data(), 3);
    ASSERT_EQ(res2, expectedRes3);

    res2 = vectorAppend(res, x.data(), 3);
    ASSERT_EQ(res2, expectedRes4);

    res2 = vectorAppend(res, y);
    ASSERT_EQ(res2, expectedRes5);
}

TEST(VectorTest, EqualityCheck) {

    Eigen::Vector3d x(1, 2, 3);
    std::vector<double> y{1, 2, 3, 4, 5, 6, 7};

    std::vector<double> res;

    vectorAppendInPlace(res, y);

    ASSERT_TRUE(vectorEquals(y, res));

    vectorAppendInPlace(res, x.data(), 3);

    ASSERT_FALSE(vectorEquals(y, res));
}

TEST(VectorTest, SpliceFunctionality) {

    int n = 10;
    std::vector<double> x(n);
    for (int i = 0; i < n; i++) {
        x[i] = i + 1;
    }

    std::vector<double> expected1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<double> expected2 = {2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> expected3 = {};
    std::vector<double> expected4 = {};
    std::vector<double> expected5 = {};

    auto result1 = spliceVector(x, 0, 9);
    ASSERT_EQ(result1, expected1);

    auto result2 = spliceVector(x, 1, 8);
    ASSERT_EQ(result2, expected2);

    auto result3 = spliceVector(x, 6, 4);
    ASSERT_EQ(result3, expected3);

    auto result4 = spliceVector(x, 0, -1);
    ASSERT_EQ(result4, expected4);

    auto result5 = spliceVector(x, 10, 9);
    ASSERT_EQ(result5, expected5);
}

TEST(VectorTest, RemovingDuplicates) {
    vector<int> x{10, 10, 10, 10, 1, 12, 13, 14, 1, 1, 1};
    set<int> s(x.begin(), x.end());
    vector<int> v(s.begin(), s.end());

   auto z = removeDuplicates(x);
    vector<int> expected{1,10,12,13,14};
    ASSERT_EQ(z, expected);

}

TEST(VectorTest, EigenMatrixConversion) {

    Eigen::Matrix<double, 2, 3> bb;
    bb << -0.113, -0.113, -0.003,
          0.113,  0.113,  0.016;

    std::vector<double> expectedVector = {-0.113, 0.113, -0.113, 0.113, -0.003, 0.016};

    auto resultVector = vectorFromArray(bb.transpose().data(), 6);

    ASSERT_EQ(resultVector, expectedVector);
}

TEST(VectorTest, BasicOperations) {

    std::vector<int> v(10);
    for (int i = 0; i < 10; ++i) {
        v[i] = i + 1;
    }


    ASSERT_EQ(v.size(), 10);

    v.reserve(20);

    ASSERT_EQ(v.size(), 10);

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(v[i], i + 1);
    }

    v.emplace_back(11);

    ASSERT_EQ(v.size(), 11);

    for (int i = 0; i < 11; ++i) {
        ASSERT_EQ(v[i], i + 1);
    }
}

TEST(VectorTest, ObjectLifecycle) {

    A **m = new2dArray<A>(3, 5);

    delete2dArray(m, 3);

    ASSERT_EQ(A::count, 0);
}

TEST(VectorTest, StdAllOfEmpty) {
    std::vector<int> x;
    EXPECT_TRUE(std::all_of(x.begin(), x.end(), [](auto const &val) { return val == 5; }));
}

TEST(VectorTest, StdAnyOfEmpty) {
    std::vector<int> x;
    EXPECT_FALSE(std::any_of(x.begin(), x.end(), [](auto const &val) { return val == 5; }));
}

int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    // testSortMultipleVectorsBasedOnOneCriterion();
    // testVectorAppendFunctions();
    // testVectorEquals();
    // testSpliceVector();
    // testRemoveDuplicates();
    // testEigenDataAsArray();
    // testResizeAndReserve();
    // testCreationOfDynamicSizeArray();


    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
