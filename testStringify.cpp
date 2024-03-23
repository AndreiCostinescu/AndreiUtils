//
// Created by Andrei on 29.11.23.
//

#include <AndreiUtils/traits/stringify.hpp>
#include <AndreiUtils/utilsString.h>
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

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

int main() {
    cout << "Hello World!" << endl;

    // testStringifyTypes();
    // testStringRemove();
    testStringEndsWith();
    testStdAllOf();
    // testStringWithoutLastParts();
    testRemoveFrom();

    return 0;
}
