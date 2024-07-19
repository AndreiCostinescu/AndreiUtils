//
// Created by Andrei on 15.04.24.
//

#include <AndreiUtils/utilsJson.hpp>
#include <gtest/gtest.h>

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

TEST(SlidingWindowSerializationTest, BasicSerialization) {
    SlidingWindow<int> w(5);
    w.addData(1);
    w.addData(2);
    nlohmann::json j = w;

    std::cout << j.dump(4) << std::endl;

    auto copyW = j.get<SlidingWindow<int>>();
    j = copyW;

    std::cout << j.dump(4) << std::endl;

}

TEST(SlidingWindowSerializationTest, WithInvalidValuesSerialization) {
    SlidingWindowWithInvalidValues<int> w(5);
    w.addData(1);
    w.addData(2);
    nlohmann::json j = w;


    std::cout << j.dump(4) << std::endl;


    w.addData(3, false);
    w.addData(4);

    j = w;
    std::cout << j.dump(4) << std::endl;

}

int main(int argc, char **argv) {
        cout << "Hello World!" << endl;

    //testSlidingWindowSerialization();
    //testSlidingWindowWithInvalidValuesSerialization();

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}