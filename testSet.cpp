//
// Created by Andrei on 24.11.23.
//

#include <AndreiUtils/utilsSet.hpp>
#include <cassert>
#include <iostream>
#include <gtest/gtest.h>

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