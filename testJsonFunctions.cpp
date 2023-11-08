//
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <AndreiUtils/classes/MixedDataContainer.hpp>
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

int main() {
    cout << "Hello World!" << endl;

    // eigenJsonTesting();
    // testJsonNull();
    // testJsonArraySerialization();
    testConfigurationParameters();

    return 0;
}
