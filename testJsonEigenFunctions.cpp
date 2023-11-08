//
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <AndreiUtils/classes/MixedDataContainer.hpp>
#include <AndreiUtils/utilsJsonEigen.hpp>
#include <iostream>

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

void testJsonNull() {
    nlohmann::json j = nullptr;
    cout << "JSON CONTENT: " << j.dump() << endl;
}

void testJsonArraySerialization() {
    nlohmann::json j = readJsonFile("../testJsonOutput.json");
    j["data"] = {110, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    j["name"] = "dummy data 2";
    j["isDummy"] = false;
    writeJsonFile("../testJsonOutput.json", j);
}

void testMixedDataContainer() {
    nlohmann::json x;
    x["24"] = 25;
    MixedDataContainer c;
    c.addData("json", &x);
    auto tmp = *(c.getData<nlohmann::json>("json"));
    cout << tmp.dump() << endl;
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

int main() {
    cout << "Hello World!" << endl;

    // eigenJsonTesting();
    // testJsonNull();
    // testJsonArraySerialization();
    testEigenFromVectorDeserialization();

    return 0;
}
