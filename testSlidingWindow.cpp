// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

    // testSlidingWindowSerialization();
    // testSlidingWindowWithInvalidValuesSerialization();

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
