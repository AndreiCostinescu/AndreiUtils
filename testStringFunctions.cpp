//
// Created by Andrei on 29.11.23.
//

#include <AndreiUtils/traits/stringify.hpp>
#include <AndreiUtils/utilsString.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <gtest/gtest.h>

using namespace std;

TEST(StringUtilsTest, SplitString) {
    string s = "ToSplit";

    vector<string> result = AndreiUtils::splitString(s, "To");
    ASSERT_EQ(result.size(), 2) << "Split on 'To' failed";

    result = AndreiUtils::splitString(s, "Spli");
    ASSERT_EQ(result.size(), 2) << "Split on 'Spli' failed";

    result = AndreiUtils::splitString(s, "Split");
    ASSERT_EQ(result.size(), 2) << "Split on 'Split' failed";

    result = AndreiUtils::splitString(s, "ToSplit");
    ASSERT_EQ(result.size(), 2) << "Split on 'ToSplit' failed";

    s = "ToSplitTo";

    result = AndreiUtils::splitString(s, "To");
    ASSERT_EQ(result.size(), 3) << "Split on 'To' failed";

    result = AndreiUtils::splitString(s, "Spli");
    ASSERT_EQ(result.size(), 2) << "Split on 'Spli' failed";

    result = AndreiUtils::splitString(s, "Split");
    ASSERT_EQ(result.size(), 2) << "Split on 'Split' failed";

    result = AndreiUtils::splitString(s, "ToSplit");
    ASSERT_EQ(result.size(), 2) << "Split on 'ToSplit' failed";

    result = AndreiUtils::splitString(s, "ToSplitTo");
    ASSERT_EQ(result.size(), 2) << "Split on 'ToSplitTo' failed";
}

TEST(StringUtilsTest, StringifyTypes) {
    int x = 5;
    int &y = x;
    int const &z = x;
    std::complex<int> w(1, 5);
    std::pair<std::complex<double>, bool> v(1.2 + 4.5j, false);
    int *a = &x;
    int *&b = a;
    int *const c = a;
    int *const &d = a;
    int const *e = a;
    int const *&f = e;
    int const *const g = a;
    int const *const &h = a;

    ASSERT_EQ(AndreiUtils::toString(x), "5");
    ASSERT_EQ(AndreiUtils::toString(y), "5");
    ASSERT_EQ(AndreiUtils::toString(z), "5");


    std::ostringstream oss;
    oss << &x;
    std::string addressString = oss.str();

    ASSERT_EQ(AndreiUtils::toString(a), addressString);
    ASSERT_EQ(AndreiUtils::toString(b), addressString);
    ASSERT_EQ(AndreiUtils::toString(c), addressString);
    ASSERT_EQ(AndreiUtils::toString(d), addressString);
    ASSERT_EQ(AndreiUtils::toString(e), addressString);
    ASSERT_EQ(AndreiUtils::toString(f), addressString);
    ASSERT_EQ(AndreiUtils::toString(g), addressString);
    ASSERT_EQ(AndreiUtils::toString(h), addressString);
    ASSERT_EQ(AndreiUtils::toString(w), "1 + i * 5");
    ASSERT_EQ(AndreiUtils::toString(v), "<1.200000 + i * 4.500000, false>");
}

TEST(StringUtilsTest, RemoveFunctions) {
    string s = "Hello World!";
    string rem = "lo World!";
    string rem2 = "Hello World";

    EXPECT_EQ(AndreiUtils::removeLeft(s, 0), s);
    EXPECT_EQ(AndreiUtils::removeRight(s, 0), s);
    EXPECT_EQ(AndreiUtils::removeLeft(s, 3), rem);
    EXPECT_EQ(AndreiUtils::removeRight(s, 1), rem2);
    EXPECT_EQ(AndreiUtils::removeLeft(s, 12), "");
    EXPECT_EQ(AndreiUtils::removeRight(s, 12), "");
}

TEST(StringUtilsTest, EndsWithInteger) {
    string s = "Instance123", rest;
    int number;

    ASSERT_TRUE(AndreiUtils::endsWithInteger(s, number, rest));
    EXPECT_EQ(number, 123);
    EXPECT_EQ(rest, "Instance");

    s = "s";
    ASSERT_FALSE(AndreiUtils::endsWithInteger(s, number, rest));

    s = "Instance0";
    ASSERT_TRUE(AndreiUtils::endsWithInteger(s, number, rest));
    EXPECT_EQ(number, 0);
    EXPECT_EQ(rest, "Instance");

    s = "HopHop-45";
    ASSERT_TRUE(AndreiUtils::endsWithInteger(s, number, rest));
    EXPECT_EQ(number, 45);
    EXPECT_EQ(rest, "HopHop-");
}

TEST(StringUtilsTest, AllOf) {
    auto isStringSizeOdd = [](std::string const &s) {
        return s.size() % 2 == 1;
    };

    vector<std::string> v(0, "s");
    auto res = std::all_of(v.begin(), v.end(), isStringSizeOdd);
    EXPECT_TRUE(res);

    v = vector<std::string>(1, "s");
    res = std::all_of(v.begin(), v.end(), isStringSizeOdd);
    EXPECT_TRUE(res);

    v = {"s", "se"};
    res = std::all_of(v.begin(), v.end(), isStringSizeOdd);
    EXPECT_FALSE(res);
}

TEST(StringUtilsTest, WithoutLastParts) {
    string s = "MilkInstance123";
    EXPECT_EQ(AndreiUtils::withoutLastParts(s, "Instance", -1), "MilkInstance123");
    EXPECT_EQ(AndreiUtils::withoutLastParts(s, "Instance", 0), "MilkInstance123");
    EXPECT_EQ(AndreiUtils::withoutLastParts(s, "Instance", 1), "Milk");
    EXPECT_EQ(AndreiUtils::withoutLastParts(s, "Instance", 2), "");
    EXPECT_EQ(AndreiUtils::withoutLastParts(s, "Instance", 3), "");

    s = "Milk123Instance123";
    EXPECT_EQ(AndreiUtils::withoutLastParts(s, "123", -1), "Milk123Instance123");
    EXPECT_EQ(AndreiUtils::withoutLastParts(s, "123", 0),  "Milk123Instance123");
    EXPECT_EQ(AndreiUtils::withoutLastParts(s, "123", 1), "Milk123Instance");
    EXPECT_EQ(AndreiUtils::withoutLastParts(s, "123", 2), "Milk");
    EXPECT_EQ(AndreiUtils::withoutLastParts(s, "123", 3), "");
}

TEST(StringUtilsTest, RemoveFromStart) {
    std::string s = "MilkInstance123";
    EXPECT_EQ(AndreiUtils::removeFromStart(s, "Milk"), "Instance123");
    EXPECT_EQ(AndreiUtils::removeFromStart(s, "ilk"), "MilkInstance123");
    EXPECT_EQ(AndreiUtils::removeFromStart(s, "MilkIn"), "stance123");

    s = "MilkInstance123";
    EXPECT_EQ(AndreiUtils::removeFromEnd(s, "123"), "MilkInstance");
    EXPECT_EQ(AndreiUtils::removeFromEnd(s, "12"), "MilkInstance123");
    EXPECT_EQ(AndreiUtils::removeFromEnd(s, "e123"), "MilkInstanc");
}

TEST(StringUtilsTest, ComplexTypes) {
    std::tuple<std::complex<double>, bool, std::vector<int>> v(1.2 + 4.5j, false, std::vector<int>{1, 2, 3, 4, 5});

    string expected = "(1.200000 + i * 4.500000, false, [1, 2, 3, 4, 5])";

    EXPECT_EQ(AndreiUtils::toString(v), expected);
    EXPECT_EQ(AndreiUtils::toString(v), expected);  // check two times!
}

int main(int argc, char **argv) {

    cout << "Hello World!" << endl;

    // testSplitString();
    // testStringifyTypes();
    // testStringRemove();
    // testStringEndsWith();
    // testStdAllOf();
    // testStringWithoutLastParts();
    // testRemoveFrom();
    // testStringifyComplexTypes();


    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}