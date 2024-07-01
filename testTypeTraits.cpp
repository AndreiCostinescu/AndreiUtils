//
// Created by Andrei on 20.11.23.
//

#include <AndreiUtils/traits/is_detected.hpp>
#include <AndreiUtils/traits/is_numeric.hpp>
#include <AndreiUtils/utils.hpp>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

class Test {
public:
    explicit Test(int &data) : a(&data) {}

    void f(double, int) {}

    void f(double, int, double) {}

    int *a;
};

class A_ {
public:
    virtual ~A_() = default;
};

class B_ : virtual public A_ {
public:
    bool operator<(B_ const &other) const {
        return true;
    }
};

class C_ : virtual public A_ {
};

class D_ : public B_, public C_ {
};

template<typename T>
struct type_name {
    static string name() { return ""; }
};

template<>
struct type_name<A_> {
    static string name() { return "A"; }
};

template<>
struct type_name<B_> {
    static string name() { return "B"; }
};

template<>
struct type_name<C_> {
    static string name() { return "C"; }
};

template<>
struct type_name<D_> {
    static string name() { return "D"; }
};

template<typename T>
struct tmpStruct {
    static string tmpF(T &t) {
        return type_name<T>::name();
    }
};

void testTypeTraitsWithPointers() {
    A_ *a;
    B_ b;
    a = &b;
    cout << tmpStruct<B_>::tmpF(b) << endl;
    cout << tmpStruct<A_ *>::tmpF(a) << endl;
}

void testIntegralAndUnsignedTypes() {
    cout << std::boolalpha;
    cout << is_integral<bool>::value << endl;
    cout << is_unsigned<bool>::value << endl;
    cout << endl;
    cout << is_integral<long long>::value << endl;
    cout << is_integral<int64_t>::value << endl;
    cout << endl;
    cout << is_integral<unsigned long long>::value << endl;
    cout << is_integral<uint64_t>::value << endl;
    cout << endl;
    cout << is_unsigned<long long>::value << endl;
    cout << is_unsigned<int64_t>::value << endl;
    cout << endl;
    cout << is_unsigned<unsigned long long>::value << endl;
    cout << is_unsigned<uint64_t>::value << endl;
    cout << endl;
}

void testIsNumericType() {
    cout << std::boolalpha;
    cout << "Is integral: " << endl;
    cout << std::is_integral_v<bool> << endl;
    cout << std::is_integral_v<int> << endl;
    cout << std::is_integral_v<double> << endl;
    cout << "Is floating: " << endl;
    cout << std::is_floating_point_v<bool> << endl;
    cout << std::is_floating_point_v<int> << endl;
    cout << std::is_floating_point_v<double> << endl;
    cout << "Is numeric: " << endl;
    cout << AndreiUtils::is_numeric_v<bool> << endl;
    cout << AndreiUtils::is_numeric_v<int> << endl;
    cout << AndreiUtils::is_numeric_v<double> << endl;
}

void testInstanceOf() {
    B_ b;
    int _tmp = 1;
    Test e(_tmp);
    A_ *a = &b;
    A_ c;

    cout << "A is polymorphic? " << is_polymorphic<A_>::value << endl;
    cout << "B is polymorphic? " << is_polymorphic<B_>::value << endl;
    cout << "Test is polymorphic? " << is_polymorphic<Test>::value << endl;
    cout << "A* is polymorphic? " << is_polymorphic<A_ *>::value << endl;
    cout << "B* is polymorphic? " << is_polymorphic<B_ *>::value << endl;
    cout << "Test* is polymorphic? " << is_polymorphic<Test *>::value << endl;

    cout << "a instance of A?: " << instanceOf<A_>(a) << endl;
    cout << "b instance of A?: " << instanceOf<A_>(b) << endl;
    cout << "c instance of A?: " << instanceOf<A_>(c) << endl;
    cout << "e instance of A?: " << instanceOf<A_>(e) << endl;
    cout << "a instance of B?: " << instanceOf<B_>(a) << endl;
    cout << "b instance of B?: " << instanceOf<B_>(b) << endl;
    cout << "c instance of B?: " << instanceOf<B_>(c) << endl;
    cout << "e instance of B?: " << instanceOf<B_>(e) << endl;
    cout << "a instance of Test?: " << instanceOf<Test>(a) << endl;
    cout << "b instance of Test?: " << instanceOf<Test>(b) << endl;
    cout << "c instance of Test?: " << instanceOf<Test>(c) << endl;
    cout << "e instance of Test?: " << instanceOf<Test>(e) << endl;
}

template<class T>
using has_f1 = decltype(std::declval<T&>().purr(std::declval<int>()));

template<class T>
using has_f2 = decltype(std::declval<T&>().f(std::declval<double>()));

template<class T>
using has_f3 = decltype(std::declval<T&>().f(std::declval<double>(), std::declval<int>()));

template<class T>
using has_f4 = decltype(std::declval<T&>().f(std::declval<double>(), std::declval<int>(), std::declval<double>()));

template<class T>
using has_f5 = decltype(std::declval<T&>().f(std::declval<double>(), std::declval<double>(), std::declval<double>()));

template<class T>
using has_f6 = decltype(std::declval<T&>().f(std::declval<double>(), std::declval<float>(), std::declval<double>()));

template<class T>
using has_f7 = decltype(std::declval<T&>().f(std::declval<double>(), std::declval<int>(), std::declval<float>()));

template<class T>
using has_f8 = decltype(std::declval<T&>().f(std::declval<Test>(), std::declval<int>(), std::declval<float>()));

template<class T>
using has_f9 = decltype(std::declval<T&>().f(std::declval<double>(), std::declval<int>(), std::declval<double>(), std::declval<float>()));

template<class T>
using has_f10 = decltype(std::declval<T&>().f(std::declval<int>()));

void testIsDetected() {
    cout << AndreiUtils::is_detected<has_f1, Test>::value << endl;
    cout << AndreiUtils::is_detected<has_f2, Test>::value << endl;
    cout << AndreiUtils::is_detected<has_f3, Test>::value << endl;
    cout << AndreiUtils::is_detected<has_f4, Test>::value << endl;
    cout << AndreiUtils::is_detected<has_f5, Test>::value << endl;
    cout << AndreiUtils::is_detected<has_f6, Test>::value << endl;
    cout << AndreiUtils::is_detected<has_f7, Test>::value << endl;
    cout << AndreiUtils::is_detected<has_f8, Test>::value << endl;
    cout << AndreiUtils::is_detected<has_f9, Test>::value << endl;
    cout << AndreiUtils::is_detected<has_f10, Test>::value << endl;
    cout << endl;
}

int main() {
    cout << "Hello World!" << endl;

    // testTypeTraitsWithPointers();
    // testIntegralAndUnsignedTypes();
    // testIsNumericType();
    // testInstanceOf();
    testIsDetected();

    return 0;
}
