//
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <AndreiUtils/classes/MixedDataContainer.hpp>
#include <AndreiUtils/classes/ParametersWithExternalConfig.hpp>
#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsJson.hpp>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

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
    cout << "Subconfig: " << config2.getJson().dump(4) << endl;

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

void testMixedDataContainer() {
    nlohmann::json x;
    x["24"] = 25;
    MixedDataContainer c;
    c.addData("json", &x);
    auto tmp = *(c.getData<nlohmann::json>("json"));
    cout << tmp.dump() << endl;
}

void testIntervalSerialization() {
    auto i0 = IntervalD::createEmpty();
    Interval<double> i1(20.5, 30.5);
    Interval<int> i2(20, 30);
    Interval<int> i3 = IntervalI::createFullRange();
    auto i4 = IntervalD::createOnlyLowerBound(-4.5);
    auto i5 = IntervalF::createOnlyUpperBound(5.4);
    auto i6 = IntervalD::createOnlyUpperBound(5.4);
    nlohmann::json j;
    j["i0"] = i0;
    j["i1"] = i1;
    j["i2"] = i2;
    j["i3"] = i3;
    j["i4"] = i4;
    j["i5"] = i5;
    j["i6"] = i6;
    cout << j.dump(4) << endl;
    auto dI0 = j.at("i0").get<IntervalD>();
    auto dI1 = j.at("i1").get<IntervalD>();
    auto dI2 = j.at("i2").get<IntervalI>();
    auto dI3 = j.at("i3").get<IntervalI>();
    auto dI4 = j.at("i4").get<IntervalD>();
    auto dI5 = j.at("i5").get<IntervalF>();
    auto dI6 = j.at("i6").get<IntervalD>();
    cout << dI0.toString() << endl;
    cout << dI1.toString() << endl;
    cout << dI2.toString() << endl;
    cout << dI3.toString() << endl;
    cout << dI4.toString() << endl;
    cout << dI5.toString() << endl;
    cout << dI6.toString() << endl;
}

void testGettingReferencesInJson() {
    nlohmann::json j;
    j["numbers"]["1"] = 42;
    j["numbers"]["2"] = "Gangnam Style";
    j["numbers"]["0"] = nullptr;
    j["numbers"]["3"] = 3.9;
    j["access"] = "denied";

    cout << "Original data: " << j.dump(4) << endl;

    // a reference is not possible...
    auto const &data = j.at("numbers").get<std::map<std::string, nlohmann::json>>();
    auto &refData = j.at("numbers");
}

void testParametersWithExternalConfigs() {
    // vector<int> indices{1, 2, 3, 4, 5, 6, 7, 8};
    // vector<int> indices{11};
    /*
    vector<int> indices{14};
    for (auto const &index: indices) {
        string fileName = "../data/testExternalConfig" + std::to_string(index) + ".json";
        ParametersWithExternalConfig p(fileName);
        cout << "P" << index << " config: " << p.getJson().dump(4) << endl;
        cout << p.toString("", true) << endl;
        // p.updateParameters();
        p.writeParameters(fileName, true);
        cout << "\n\n";
    }
    //*/

    /*
    string fileName = "../data/testExternalConfig14.json";
    ParametersWithExternalConfig p(fileName);
    cout << p.toString("", true) << endl;
    p.at("external").at("external").at("external").at("external").at("dependencies").set<string>("name", "modified x13 data");
    cout << p.toString("", true) << endl;
    p.writeParameters(fileName, true, true);
    cout << "\n\n";
    //*/

    string fileName = "../data/testExternalConfig5.json";
    ParametersWithExternalConfig p(fileName);
    p.writeParameters(fileName, true, true);
    cout << "\n\n";
}

void testWriteJsonWithKeepFormatAndNewLines() {
    string filePath = "../data/testConfig1.json";
    writeJsonFileKeepOrder(filePath, readJsonFile(filePath), true);
}

int main() {
    cout << "Hello World!" << endl;

    // testJsonNull();
    // testJsonArraySerialization();
    // testConfigurationParameters();
    // testMixedDataContainer();
    // testIntervalSerialization();
    // testParametersWithExternalConfigs();
    testWriteJsonWithKeepFormatAndNewLines();

    return 0;
}
