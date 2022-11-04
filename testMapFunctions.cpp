//
// Created by Andrei on 03.11.22.
//

#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsVector.hpp>

using namespace AndreiUtils;
using namespace std;

void testMapKeys() {
    map<string, int> x{
            {"72", 1},
            {"60", 2},
            {"48", 3},
            {"36", 4},
            {"24", 5},
            {"12", 6},
    };
    printVector(getMapKeys(x));
}

void testMapFiltering() {
    map<int, int> x;
    for (int i = 0; i < 20; i++) {
        x[i] = i - 5;
    }
    auto y = getFilteredMapBasedOnPredicate<int, int>(x, [](const int &key, const int &value) {
        // return false;
        // return true;
        // return key % 3;
        return value % 4 != 0;
    });
    cout << "Printing map:" << endl;
    printMap(x);
    printMap(y);
    cout << "Done!" << endl;
}

void testMapRefAccessing() {
    map<int, int> x;
    x[0] = 0;
    x[1] = 1;
    x[2] = 2;
    x[3] = 3;
    x[4] = 4;
    int value = 42;
    int *valuePtr = nullptr;
    int const *valueConstPtr = nullptr;

    // ------------------ CHECK MAP_GET_IF_CONTAINS FUNCTIONS ------------------

    cout << mapGetIfContains(x, -1, value) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, -1, valuePtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, -1, valueConstPtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, 0, value) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, 1, valuePtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, 2, valueConstPtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    value = 42;

    cout << mapGetIfContains(x, 1, valuePtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << "Update valuePtr..." << endl;
    *valuePtr = 42;
    cout << printMapToString(x) << endl;
    cout << endl;
    int &valueRef = *valuePtr;

    cout << mapGetIfContains(x, 2, valuePtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << "Update valueRef..." << endl;
    valueRef = 69;
    *valuePtr = -42;
    cout << printMapToString(x) << endl;
    cout << endl;

    // ------------------ NOW CHECK MAP_GET FUNCTIONS ------------------

    try {
        mapGet(x, -1);
    } catch (exception &e) {
        if (strcmp(e.what(), "Element not found in map!") != 0) {
            throw e;
        }
    }

    value = mapGet(x, 0);
    cout << "value = " << value << endl;
    value = 5;
    cout << printMapToString(x) << endl;
    cout << endl;

    int &valueRef2 = mapGet(x, 4);
    cout << "valueRef2 = " << valueRef2 << endl;
    valueRef2 = 5;
    cout << printMapToString(x) << endl;
    cout << endl;

    int &valueRef3 = mapGet(x, 1);
    valueRef3 = -14;
    cout << "valueRef3 = " << valueRef3 << endl;
    cout << "valueRef  = " << valueRef << endl;
    cout << printMapToString(x) << endl;
    cout << endl;
}

void testMapCopy() {
    map<int, int> x, y;
    x[0] = 0;
    x[1] = 1;
    x[2] = 1;
    y = x;
    y[0] = 42;

    printMap(x);
    cout << endl << endl;
    printMap(y);
}

class MapEmplaceTestClass {
public:
    double a, b;
    int c;

    MapEmplaceTestClass(double a, double b, int c) : a(a), b(b), c(c) {}
};

void testMapEmplace() {
    using TMap = map<int, MapEmplaceTestClass>;
    TMap t;
    MapEmplaceTestClass s(10.0, -6.9, 3);
    t.insert(std::make_pair(1, std::forward<typename TMap::mapped_type>(s)));
    // TMap::value_type p = std::make_pair(1, s);
    // t.insert(std::move(p));
    // t[1] = s;

    map<int, MapEmplaceTestClass> m;
    mapAdd(m, 42, {1, 2, 3});

    auto x = mapEmplace(m, 0, 24., 48., 10);
    cout << x->first << endl;
    cout << x->second.a << ", " << x->second.b << ", " << x->second.c << endl;
    x->second.a *= 2;
    x->second.b /= 4;
    x->second.c += 5;
    printMapConvertValue<int, MapEmplaceTestClass>(m, [](MapEmplaceTestClass const &_x) {
        return "{" + to_string(_x.a) + ", " + to_string(_x.b) + ", " + to_string(_x.c) + "}";
    });

    map<char, int> mymap;
    mymap.emplace('x', 100);
    mymap.emplace('y', 200);
    mymap.emplace('z', 100);

    std::cout << "mymap contains:";
    printMap(mymap);
}

int main() {
    cout << "Hello World!" << endl;

    // testMapKeys();
    // testMapFiltering();
    // testMapRefAccessing();
    // testMapCopy();
    testMapEmplace();

    return 0;
}