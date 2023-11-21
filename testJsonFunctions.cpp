//
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <AndreiUtils/classes/MixedDataContainer.hpp>
#include <AndreiUtils/utilsJson.hpp>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

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

int main() {
    cout << "Hello World!" << endl;

    // eigenJsonTesting();
    // testJsonNull();
    // testJsonArraySerialization();
    // testConfigurationParameters();
    testIntervalSerialization();

    return 0;
}
