//
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <AndreiUtils/classes/MixedDataContainer.hpp>
// #include <AndreiUtils/utilsJsonEigen.hpp>
#include <iostream>

using namespace AndreiUtils;
// using namespace Eigen;
using namespace std;

/*
void eigenJsonTesting() {
    Matrix2i m = Matrix2i::Identity();
    m(0, 1) = 2;
    nlohmann::json j = m;
    cout << "Json from matrix m " << endl << m << endl << j << endl;
    Matrix2i m2 = j.get<MatrixXi>();
    cout << "Reconstructed matrix:" << endl << m2 << endl;

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

//*/
void testConfigurationParameters() {
    nlohmann::json j, k;
    j["parametersFor"] = "Test";

    map<int, int> x;
    x[0] = 1;
    x[1] = 1;
    x[2] = 1;
    k["actions"] = x;
    k["parametersFor"] = "SubConfig";
    j["SubConfig"] = k;

    ConfigurationParameters c(j, "Test");

    /* getSubConfig creates a temporary object; don't get the result of the getJson() function into a reference val! */

    auto const &config = c.getSubConfig("SubConfig");
    auto &tmp2 = config.getJson("actions");
    cout << tmp2.dump(4) << endl << endl;

    auto tmp3 = c.getSubConfig("SubConfig").getJson("actions");
    cout << tmp3.dump(4) << endl << endl;

    auto const &config2 = c.getSubConfig("SubConfig");
    auto const &tmp5 = config.getJson("actions");
    cout << "TMP5: " << tmp5.dump(4) << endl << endl;

    nlohmann::json tmp6 = c.getSubConfig("SubConfig").getJson("actions");
    cout << "TMP6: " << tmp6.dump(4) << endl;

    auto const &tmp7(c.getSubConfig("SubConfig").getJson("actions"));
    cout << "TMP7: " << tmp7.dump(4) << endl;

    auto &tmp = c.getSubConfig("SubConfig").getJson("actions");
    cout << "TMP1: " << tmp.dump(4) << endl;

    auto const &tmp4 = c.getSubConfig("SubConfig").getJson("actions");
    cout << tmp4.dump(4) << endl;
}

int main() {
    cout << "Hello World!" << endl;

    // eigenJsonTesting();
    // testJsonNull();
    // testJsonArraySerialization();
    testConfigurationParameters();

    return 0;
}
