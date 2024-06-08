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

template<typename T>
std::string pointerValueToString(T *p) {
    return (p == nullptr ? "null" : std::to_string(*p));
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
    cout << value << ", " << pointerValueToString(valuePtr) << ", " << pointerValueToString(valueConstPtr) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, -1, valuePtr) << endl;
    cout << value << ", " << pointerValueToString(valuePtr) << ", " << pointerValueToString(valueConstPtr) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, -1, valueConstPtr) << endl;
    cout << value << ", " << pointerValueToString(valuePtr) << ", " << pointerValueToString(valueConstPtr) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, 0, value) << endl;
    cout << value << ", " << pointerValueToString(valuePtr) << ", " << pointerValueToString(valueConstPtr) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, 1, valuePtr) << endl;
    cout << value << ", " << pointerValueToString(valuePtr) << ", " << pointerValueToString(valueConstPtr) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, 2, valueConstPtr) << endl;
    cout << value << ", " << pointerValueToString(valuePtr) << ", " << pointerValueToString(valueConstPtr) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    value = 42;

    cout << mapGetIfContains(x, 1, valuePtr) << endl;
    cout << value << ", " << pointerValueToString(valuePtr) << ", " << pointerValueToString(valueConstPtr) << endl;
    cout << "Update valuePtr..." << endl;
    *valuePtr = 42;
    cout << printMapToString(x) << endl;
    cout << endl;
    int &valueRef = *valuePtr;

    cout << mapGetIfContains(x, 2, valuePtr) << endl;
    cout << value << ", " << pointerValueToString(valuePtr) << ", " << pointerValueToString(valueConstPtr) << endl;
    cout << "Update valueRef..." << endl;
    valueRef = 69;
    *valuePtr = -42;
    cout << printMapToString(x) << endl;
    cout << endl;

    // ------------------ NOW CHECK MAP_GET FUNCTIONS ------------------

    try {
        value = mapGet(x, -1);
    } catch (exception &e) {
        if (strcmp(e.what(), "Element -1 not found in map!") != 0) {
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

    MapEmplaceTestClass() : a(), b(), c() {
        cout << "Empty constructor..." << endl;
    }

    MapEmplaceTestClass(double a, double b, int c) : a(a), b(b), c(c) {}

    MapEmplaceTestClass(MapEmplaceTestClass const &other) : a(other.a), b(other.b), c(other.c) {
        cout << "Copy constructor..." << endl;
    }

    MapEmplaceTestClass(MapEmplaceTestClass &&other) noexcept: a(other.a), b(other.b), c(other.c) {
        cout << "Move constructor..." << endl;
    }

    MapEmplaceTestClass &operator=(MapEmplaceTestClass const &other) {
        cout << "Copy assignment..." << endl;
        if (this != &other) {
            this->a = other.a;
            this->b = other.b;
            this->c = other.c;
        }
        return *this;
    }

    MapEmplaceTestClass &operator=(MapEmplaceTestClass &&other) noexcept {
        cout << "Move assignment..." << endl;
        if (this != &other) {
            this->a = other.a;
            this->b = other.b;
            this->c = other.c;
        }
        return *this;
    }
};

class MapEmplaceTestTestClass {
public:
    std::string name;
    MapEmplaceTestClass tmp;

    MapEmplaceTestTestClass(std::string name, MapEmplaceTestClass tmp) : name(std::move(name)), tmp(std::move(tmp)) {}
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

void testMapEmplaceMoveCopy() {
    using TMap = map<int, MapEmplaceTestClass>;
    TMap t;

    MapEmplaceTestClass s(10.0, -6.9, 3);
    cout << "Point1" << endl;
    t.insert(std::make_pair(1, std::forward<typename TMap::mapped_type>(s)));
    cout << "Point2" << endl;

    TMap m;
    cout << "Point3" << endl;
    mapAdd(m, 42, {1, 2, 3});
    cout << "Point4" << endl;
    cout << "Point5" << endl;
    auto x = mapEmplace(m, 0, 24., 48., 10);
    cout << "Point6" << endl;
    cout << x->first << endl;
    cout << x->second.a << ", " << x->second.b << ", " << x->second.c << endl;
    x->second.a *= 2;
    x->second.b /= 4;
    x->second.c += 5;
    auto printer = [](MapEmplaceTestClass const &_x) -> std::string {
        return "{" + to_string(_x.a) + ", " + to_string(_x.b) + ", " + to_string(_x.c) + "}";
    };
    printMapConvertValue<int, MapEmplaceTestClass>(m, printer);

    MapEmplaceTestClass s1(10.0, -6.9, 3);
    cout << "Point7" << endl;
    mapEmplace(t, -12, std::move(s1));
    cout << "Point8" << endl;
    cout << "Point9" << endl;
    mapEmplace(t, -13, std::forward<MapEmplaceTestClass>(s1));
    cout << "Point10" << endl;
    printMapConvertValue<int, MapEmplaceTestClass>(t, printer);

    cout << "Point11" << endl;
    t.emplace(std::pair<int, MapEmplaceTestClass>{-14, std::move(s1)});
    cout << "Point12" << endl;
    printMapConvertValue<int, MapEmplaceTestClass>(t, printer);

    cout << "Point13" << endl;
    t.emplace(std::piecewise_construct, std::forward_as_tuple(1), std::forward_as_tuple(std::move(s1)));
    cout << "Point14" << endl;
    printMapConvertValue<int, MapEmplaceTestClass>(t, printer);
}

void createTmp(map<std::string, MapEmplaceTestTestClass> &t, std::string name, MapEmplaceTestClass tmp) {
    // mapEmplace<string>(t, "Op1", std::move(name), std::move(tmp));
    t.emplace(std::piecewise_construct, std::forward_as_tuple("key"),
              std::forward_as_tuple(std::forward<std::string>(name), std::forward<MapEmplaceTestClass>(tmp)));
}

void testMapEmplaceMoveCopy2() {
    using TMap = map<std::string, MapEmplaceTestTestClass>;
    TMap t;

    MapEmplaceTestClass tmp(1, 2, 3);
    std::string name = "Name";
    mapEmplace(t, name, name, std::move(tmp));

    tmp = MapEmplaceTestClass(2, 3, 4);
    createTmp(t, "Name 2", std::move(tmp));
}

void testSingleElementFunctions() {
    int key, value;
    try {
        std::map<int, int> m = {{1, 42}, {4, 1}};
        getSingleElement(m, key, value);
    } catch (std::runtime_error &e) {
        assert(std::string(e.what()) == "Map does not have only one element (2)!");
    }
    try {
        std::map<int, int> m;
        getSingleElement(m, key, value);
    } catch (std::runtime_error &e) {
        assert(std::string(e.what()) == "Map does not have only one element (0)!");
    }
    std::map<int, int> m = {{1, 42}};
    getSingleElement(m, key, value);
    assert(key == 1);
    assert(value == 42);
}

void testMapAddIfNotContains() {
    map<int, string> x = {{1, "1"}, {0, "2"}};
    printMap(x);
    string *newElemAddr = mapAddIfNotContains<int, string>(x, 2, "0");
    if (newElemAddr != &mapGet(x, 2)) {
        throw std::runtime_error("Address of new element is wrong!");
    }
    *newElemAddr += "newElem";
    printMap(x);
}

int main() {
    cout << "Hello World!" << endl;

    // testMapKeys();
    // testMapFiltering();
    // testMapRefAccessing();
    // testMapCopy();
    // testMapEmplace();
    // testMapEmplaceMoveCopy();
    // testMapEmplaceMoveCopy2();
    // testSingleElementFunctions();
    testMapAddIfNotContains();

    return 0;
}