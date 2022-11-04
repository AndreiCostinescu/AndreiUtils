//
// Created by Andrei on 03.11.22.
//

#include <AndreiUtils/utilsVector.hpp>
#include <Eigen/Dense>
#include <random>

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
}

int main() {
    cout << "Hello World!" << endl;

    // testSortMultipleVectorsBasedOnOneCriterion();
    // testVectorAppendFunctions();
    // testVectorEquals();
    // testSpliceVector();

    return 0;
}