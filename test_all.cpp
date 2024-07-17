//
// Created by melisa on 7/16/24.
//
// Main function to run all tests
#include <AndreiUtils/utilsJson.hpp>
#include <gtest/gtest.h>

using namespace AndreiUtils;
using namespace std;


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}