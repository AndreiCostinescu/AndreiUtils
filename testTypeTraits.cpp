//
// Created by Andrei on 20.11.23.
//

#include <AndreiUtils/traits/is_detected.hpp>
#include <AndreiUtils/traits/is_numeric.hpp>
#include <AndreiUtils/utils.hpp>
#include <iostream>
#include <gtest/gtest.h>

using namespace AndreiUtils;
using namespace std;

class MyTest {
public:
    explicit MyTest(int &data) : a(&data) {}

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
    MyTest e(_tmp);
    A_ *a = &b;
    A_ c;

    cout << "A is polymorphic? " << is_polymorphic<A_>::value << endl;
    cout << "B is polymorphic? " << is_polymorphic<B_>::value << endl;
    cout << "Test is polymorphic? " << is_polymorphic<MyTest>::value << endl;
    cout << "A* is polymorphic? " << is_polymorphic<A_ *>::value << endl;
    cout << "B* is polymorphic? " << is_polymorphic<B_ *>::value << endl;
    cout << "Test* is polymorphic? " << is_polymorphic<MyTest *>::value << endl;

    cout << "a instance of A?: " << instanceOf<A_>(a) << endl;
    cout << "b instance of A?: " << instanceOf<A_>(b) << endl;
    cout << "c instance of A?: " << instanceOf<A_>(c) << endl;
    cout << "e instance of A?: " << instanceOf<A_>(e) << endl;
    cout << "a instance of B?: " << instanceOf<B_>(a) << endl;
    cout << "b instance of B?: " << instanceOf<B_>(b) << endl;
    cout << "c instance of B?: " << instanceOf<B_>(c) << endl;
    cout << "e instance of B?: " << instanceOf<B_>(e) << endl;
    cout << "a instance of Test?: " << instanceOf<MyTest>(a) << endl;
    cout << "b instance of Test?: " << instanceOf<MyTest>(b) << endl;
    cout << "c instance of Test?: " << instanceOf<MyTest>(c) << endl;
    cout << "e instance of Test?: " << instanceOf<MyTest>(e) << endl;
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
using has_f8 = decltype(std::declval<T&>().f(std::declval<MyTest>(), std::declval<int>(), std::declval<float>()));

template<class T>
using has_f9 = decltype(std::declval<T&>().f(std::declval<double>(), std::declval<int>(), std::declval<double>(), std::declval<float>()));

template<class T>
using has_f10 = decltype(std::declval<T&>().f(std::declval<int>()));

void testIsDetected() {
    cout << AndreiUtils::is_detected<has_f1, MyTest>::value << endl;
    cout << AndreiUtils::is_detected<has_f2, MyTest>::value << endl;
    cout << AndreiUtils::is_detected<has_f3, MyTest>::value << endl;
    cout << AndreiUtils::is_detected<has_f4, MyTest>::value << endl;
    cout << AndreiUtils::is_detected<has_f5, MyTest>::value << endl;
    cout << AndreiUtils::is_detected<has_f6, MyTest>::value << endl;
    cout << AndreiUtils::is_detected<has_f7, MyTest>::value << endl;
    cout << AndreiUtils::is_detected<has_f8, MyTest>::value << endl;
    cout << AndreiUtils::is_detected<has_f9, MyTest>::value << endl;
    cout << AndreiUtils::is_detected<has_f10, MyTest>::value << endl;
    cout << endl;
}

TEST(TypeTraitsTest, Pointers) {
    B_ b;
    A_* a = &b;
    EXPECT_EQ(tmpStruct<B_>::tmpF(b), "B");
    EXPECT_EQ(tmpStruct<A_*>::tmpF(a), "");
}

TEST(TypeTraitsTest, IntegralAndUnsignedTypes) {
    EXPECT_TRUE(std::is_integral<bool>::value);
    EXPECT_TRUE(std::is_unsigned<bool>::value);


    EXPECT_TRUE(std::is_integral<long long>::value);
    EXPECT_TRUE(std::is_integral<int64_t>::value);

    EXPECT_TRUE(std::is_integral<unsigned long long>::value);
    EXPECT_TRUE(std::is_integral<uint64_t>::value);

    EXPECT_FALSE(std::is_unsigned<long long>::value);
    EXPECT_FALSE(std::is_unsigned<int64_t>::value);

    EXPECT_TRUE(std::is_unsigned<unsigned long long>::value);
    EXPECT_TRUE(std::is_unsigned<uint64_t>::value);
}

TEST(TypeTraitsTest, IsNumericType) {
    EXPECT_TRUE(std::is_integral_v<bool>);
    EXPECT_TRUE(std::is_integral_v<int>);
    EXPECT_FALSE(std::is_integral_v<double>);

    EXPECT_FALSE(std::is_floating_point_v<bool>);
    EXPECT_FALSE(std::is_floating_point_v<int>);
    EXPECT_TRUE(std::is_floating_point_v<double>);

    EXPECT_TRUE(AndreiUtils::is_numeric_v<bool>);
    EXPECT_TRUE(AndreiUtils::is_numeric_v<int>);
    EXPECT_TRUE(AndreiUtils::is_numeric_v<double>);
}

TEST(TypeTraitsTest, InstanceOfCheck) {
    B_ b;
    int _tmp = 1;
    MyTest e(_tmp);
    A_ *a = &b;
    A_ c;

    EXPECT_TRUE(std::is_polymorphic_v<A_>);
    EXPECT_TRUE(std::is_polymorphic_v<B_>);
    EXPECT_FALSE(std::is_polymorphic_v<MyTest>);
    EXPECT_FALSE(std::is_polymorphic_v<A_ *>);
    EXPECT_FALSE(std::is_polymorphic_v<B_ *>);
    EXPECT_FALSE(std::is_polymorphic_v<MyTest *>);

    EXPECT_TRUE(instanceOf<A_>(a));
    EXPECT_TRUE(instanceOf<A_>(b));
    EXPECT_TRUE(instanceOf<A_>(c));
    EXPECT_FALSE(instanceOf<A_>(e));

    EXPECT_TRUE(instanceOf<B_>(a));
    EXPECT_TRUE(instanceOf<B_>(b));
    EXPECT_FALSE(instanceOf<B_>(c));
    EXPECT_FALSE(instanceOf<B_>(e));

    EXPECT_FALSE(instanceOf<MyTest>(a));
    EXPECT_FALSE(instanceOf<MyTest>(b));
    EXPECT_FALSE(instanceOf<MyTest>(c));
    EXPECT_TRUE(instanceOf<MyTest>(e));
    EXPECT_TRUE(instanceOf<MyTest>(&e));
}

TEST(TypeTraitsTest, TestFunctionDetection) {
    EXPECT_FALSE((AndreiUtils::is_detected<has_f1, MyTest>::value));
    EXPECT_FALSE((AndreiUtils::is_detected<has_f2, MyTest>::value));
    EXPECT_TRUE((AndreiUtils::is_detected<has_f3, MyTest>::value));
    EXPECT_TRUE((AndreiUtils::is_detected<has_f4, MyTest>::value));
    EXPECT_TRUE((AndreiUtils::is_detected<has_f5, MyTest>::value));
    EXPECT_TRUE((AndreiUtils::is_detected<has_f6, MyTest>::value));
    EXPECT_TRUE((AndreiUtils::is_detected<has_f7, MyTest>::value));
    EXPECT_FALSE((AndreiUtils::is_detected<has_f8, MyTest>::value));
    EXPECT_FALSE((AndreiUtils::is_detected<has_f9, MyTest>::value));
    EXPECT_FALSE((AndreiUtils::is_detected<has_f10, MyTest>::value));
}

int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    // testTypeTraitsWithPointers();
    // testIntegralAndUnsignedTypes();
    // testIsNumericType();
    // testInstanceOf();
    // testIsDetected();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}