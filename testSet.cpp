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
// Created by Andrei on 24.11.23.
//

#include <AndreiUtils/utilsSet.hpp>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

void testSetCreation() {
    auto s = setFromVector(std::vector<string>{"1", "2", "3", "2"});
    cout << s.size() << endl;
    assert(s.size() == 3);
    printSet(s);

    s = setFromMapKeys(std::map<string, int>{{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}});
    cout << s.size() << endl;
    assert(s.size() == 4);
    printSet(s);
}

TEST(SetCreationTest, Test) {
    auto s = setFromVector(std::vector<string>{"1", "2", "3", "2"});
    EXPECT_EQ(s.size(), 3);

    set<string> expected = {"1", "2", "3"};
    EXPECT_EQ(s, expected);

    s = setFromMapKeys(std::map<string, int>{{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}});
    EXPECT_EQ(s.size(), 4);

    expected = {"1", "2", "3", "4"};
    EXPECT_EQ(s, expected);
}

int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    testSetCreation();
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
