//
// Created by Andrei on 15.04.24.
//

#include <AndreiUtils/utilsJson.hpp>

using namespace AndreiUtils;
using namespace std;

void testSlidingWindowSerialization() {
    SlidingWindow<int> w(5);
    w.addData(1);
    w.addData(2);
    nlohmann::json j = w;
    cout << j.dump(4) << endl;
    w.addData(3);
    w.addData(4);
    j = w;
    cout << j.dump(4) << endl;
    w.addData(5);
    j = w;
    cout << j.dump(4) << endl;
    w.addData(6);
    j = w;
    cout << j.dump(4) << endl;

    auto copyW = j.get<SlidingWindow<int>>();
    j = copyW;
    cout << j.dump(4) << endl;

    copyW.addData(7);
    j = copyW;
    cout << j.dump(4) << endl;
}

void testSlidingWindowWithInvalidValuesSerialization() {
    SlidingWindowWithInvalidValues<int> w(5);
    w.addData(1);
    w.addData(2);
    nlohmann::json j = w;
    cout << j.dump(4) << endl;
    w.addData(3, false);
    w.addData(4);
    j = w;
    cout << j.dump(4) << endl;
    w.addData(5, false);
    j = w;
    cout << j.dump(4) << endl;
    w.addData(6);
    j = w;
    cout << j.dump(4) << endl;

    auto copyW = j.get<SlidingWindowWithInvalidValues<int>>();
    j = copyW;
    cout << j.dump(4) << endl;

    copyW.addData(7, false);
    j = copyW;
    cout << j.dump(4) << endl;
}

int main() {
    cout << "Hello World!" << endl;

    testSlidingWindowSerialization();
    testSlidingWindowWithInvalidValuesSerialization();

    return 0;
}