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

void testSplitString() {
    string s = "ToSplit";
    assert(AndreiUtils::splitString(s, "To").size() == 2);
    assert(AndreiUtils::splitString(s, "Spli").size() == 2);
    assert(AndreiUtils::splitString(s, "Split").size() == 2);
    assert(AndreiUtils::splitString(s, "ToSplit").size() == 2);
    s = "ToSplitTo";
    assert(AndreiUtils::splitString(s, "To").size() == 3);
    assert(AndreiUtils::splitString(s, "Spli").size() == 2);
    assert(AndreiUtils::splitString(s, "Split").size() == 2);
    assert(AndreiUtils::splitString(s, "ToSplit").size() == 2);
    assert(AndreiUtils::splitString(s, "ToSplitTo").size() == 2);
}

void testStringifyTypes() {
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
    cout << AndreiUtils::toString(x) << endl;
    cout << AndreiUtils::toString(y) << endl;
    cout << AndreiUtils::toString(z) << endl;
    cout << "x address = " << (&x) << endl;
    cout << AndreiUtils::toString(a) << endl;
    cout << AndreiUtils::toString(b) << endl;
    cout << AndreiUtils::toString(c) << endl;
    cout << AndreiUtils::toString(d) << endl;
    cout << AndreiUtils::toString(e) << endl;
    cout << AndreiUtils::toString(f) << endl;
    cout << AndreiUtils::toString(g) << endl;
    cout << AndreiUtils::toString(h) << endl;
    cout << AndreiUtils::toString(w) << endl;
    cout << AndreiUtils::toString(v) << endl;
}

void testStringRemove() {
    string s = "Hello World!";
    string rem = "lo World!";
    string rem2 = "Hello World";
    assert(AndreiUtils::removeLeft(s, 0) == s);
    assert(AndreiUtils::removeRight(s, 0) == s);
    assert(AndreiUtils::removeLeft(s, 3) == rem);
    assert(AndreiUtils::removeRight(s, 1) == rem2);
    assert(AndreiUtils::removeLeft(s, 12).empty());
    assert(AndreiUtils::removeRight(s, 12).empty());
}

void testStringEndsWith() {
    string s = "Instance123", rest;
    int number;
    assert(AndreiUtils::endsWithInteger(s, number, rest));
    cout << s << " ends with integer " << number << " (rest = " << rest << ")" << endl;
    assert(!AndreiUtils::endsWithInteger("s", number, rest));
    s = "Instance0";
    assert(AndreiUtils::endsWithInteger(s, number, rest));
    cout << s << " ends with integer " << number << " (rest = " << rest << ")" << endl;
    s = "HopHop-45";
    assert(AndreiUtils::endsWithInteger(s, number, rest));
    cout << s << " ends with integer " << number << " (rest = " << rest << ")" << endl;
}

void testStdAllOf() {
    std::vector<std::string> v(0, "s");
    auto res = std::all_of(v.begin(), v.end(), [](std::string const &s) {
        cout << "Checking " << s << endl;
        return s.size() % 2;
    });
    cout << "all of res = " << res << endl;
}

void testStringWithoutLastParts() {
    string s = "MilkInstance123";
    cout << s << " -> " << AndreiUtils::withoutLastParts(s, "Instance", -1) << endl;
    cout << s << " -> " << AndreiUtils::withoutLastParts(s, "Instance", 0) << endl;
    cout << s << " -> " << AndreiUtils::withoutLastParts(s, "Instance", 1) << endl;
    cout << s << " -> " << AndreiUtils::withoutLastParts(s, "Instance", 2) << endl;
    cout << s << " -> " << AndreiUtils::withoutLastParts(s, "Instance", 3) << endl;
    cout << endl;
    s = "Milk123Instance123";
    cout << s << " -> " << AndreiUtils::withoutLastParts(s, "123", -1) << endl;
    cout << s << " -> " << AndreiUtils::withoutLastParts(s, "123", 0) << endl;
    cout << s << " -> " << AndreiUtils::withoutLastParts(s, "123", 1) << endl;
    cout << s << " -> " << AndreiUtils::withoutLastParts(s, "123", 2) << endl;
    cout << s << " -> " << AndreiUtils::withoutLastParts(s, "123", 3) << endl;
}

void testRemoveFrom() {
    string s = "MilkInstance123";
    cout << s << " -> " << AndreiUtils::removeFromStart(s, "Milk") << endl;
    cout << s << " -> " << AndreiUtils::removeFromStart(s, "ilk") << endl;
    cout << s << " -> " << AndreiUtils::removeFromStart(s, "MilkIn") << endl;
    cout << s << " -> " << AndreiUtils::removeFromEnd(s, "123") << endl;
    cout << s << " -> " << AndreiUtils::removeFromEnd(s, "12") << endl;
    cout << s << " -> " << AndreiUtils::removeFromEnd(s, "e123") << endl;
}

void testStringifyComplexTypes() {
    std::tuple<std::complex<double>, bool, std::vector<int>> v(1.2 + 4.5j, false, std::vector<int>{1, 2, 3, 4, 5});

    cout << AndreiUtils::toString(v) << endl;
    cout << AndreiUtils::toString(v) << endl;
}
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
    vector<std::string> v(0, "s");

    auto res = std::all_of(v.begin(), v.end(), [](const std::string &s) {
        return s.size() % 2 == 1;
    });

    EXPECT_TRUE(res);
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