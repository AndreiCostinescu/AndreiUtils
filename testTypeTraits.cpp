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
    static string tmpF(T const &t) {
        return type_name<T>::name();
    }
};

template<class T>
using has_f1 = decltype(std::declval<T &>().purr(std::declval<int>()));

template<class T>
using has_f2 = decltype(std::declval<T &>().f(std::declval<double>()));

template<class T>
using has_f3 = decltype(std::declval<T &>().f(std::declval<double>(), std::declval<int>()));

template<class T>
using has_f4 = decltype(std::declval<T &>().f(std::declval<double>(), std::declval<int>(), std::declval<double>()));

template<class T>
using has_f5 = decltype(std::declval<T &>().f(std::declval<double>(), std::declval<double>(), std::declval<double>()));

template<class T>
using has_f6 = decltype(std::declval<T &>().f(std::declval<double>(), std::declval<float>(), std::declval<double>()));

template<class T>
using has_f7 = decltype(std::declval<T &>().f(std::declval<double>(), std::declval<int>(), std::declval<float>()));

template<class T>
using has_f8 = decltype(std::declval<T &>().f(std::declval<MyTest>(), std::declval<int>(), std::declval<float>()));

template<class T>
using has_f9 = decltype(std::declval<T &>().f(std::declval<double>(), std::declval<int>(), std::declval<double>(),
                                              std::declval<float>()));

template<class T>
using has_f10 = decltype(std::declval<T &>().f(std::declval<int>()));

TEST(TypeTraitsTest, Pointers) {
    B_ b;
    A_ *a = &b;
    EXPECT_EQ(tmpStruct<B_>::tmpF(b), "B");
    EXPECT_EQ(tmpStruct<A_ *>::tmpF(a), "");
    EXPECT_EQ(tmpStruct<A_>::tmpF(A_()), "A");
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
    B_ const bConst;
    int _tmp = 1;
    MyTest e(_tmp);
    MyTest const f(_tmp);
    A_ *a = &b;
    A_ const *aConst = &bConst;
    A_ c;
    A_ const cConst;

    EXPECT_TRUE(std::is_polymorphic_v<A_>);
    EXPECT_TRUE(std::is_polymorphic_v<B_>);
    EXPECT_FALSE(std::is_polymorphic_v<MyTest>);
    EXPECT_FALSE(std::is_polymorphic_v<A_ *>);
    EXPECT_FALSE(std::is_polymorphic_v<B_ *>);
    EXPECT_FALSE(std::is_polymorphic_v<MyTest *>);

    EXPECT_TRUE(instanceOf<A_>(a));
    EXPECT_TRUE(instanceOf<A_>(aConst));
    EXPECT_TRUE(instanceOf<A_>(b));
    EXPECT_TRUE(instanceOf<A_>(bConst));
    EXPECT_TRUE(instanceOf<A_>(c));
    EXPECT_TRUE(instanceOf<A_>(cConst));
    EXPECT_FALSE(instanceOf<A_>(e));

    EXPECT_TRUE(instanceOf<B_>(a));
    EXPECT_TRUE(instanceOf<B_>(aConst));
    EXPECT_TRUE(instanceOf<B_>(b));
    EXPECT_TRUE(instanceOf<B_>(bConst));
    EXPECT_FALSE(instanceOf<B_>(c));
    EXPECT_FALSE(instanceOf<B_>(cConst));
    EXPECT_FALSE(instanceOf<B_>(e));

    EXPECT_FALSE(instanceOf<MyTest>(a));
    EXPECT_FALSE(instanceOf<MyTest>(aConst));
    EXPECT_FALSE(instanceOf<MyTest>(b));
    EXPECT_FALSE(instanceOf<MyTest>(bConst));
    EXPECT_FALSE(instanceOf<MyTest>(c));
    EXPECT_FALSE(instanceOf<MyTest>(cConst));
    EXPECT_TRUE(instanceOf<MyTest>(e));
    EXPECT_TRUE(instanceOf<MyTest>(&e));
    EXPECT_TRUE(instanceOf<MyTest>(f));
    EXPECT_TRUE(instanceOf<MyTest>(&f));
}

TEST(TypeTraitsTest, InstanceOfCheckConst) {
    B_ b;
    B_ const bConst;
    int _tmp = 1;
    MyTest e(_tmp);
    MyTest const f(_tmp);
    A_ *a = &b;
    A_ const *aConst = &bConst;
    A_ c;
    A_ const cConst;

    EXPECT_TRUE(std::is_polymorphic_v<A_ const>);
    EXPECT_TRUE(std::is_polymorphic_v<B_ const>);
    EXPECT_FALSE(std::is_polymorphic_v<MyTest const>);
    EXPECT_FALSE(std::is_polymorphic_v<A_ const *>);
    EXPECT_FALSE(std::is_polymorphic_v<B_ const *>);
    EXPECT_FALSE(std::is_polymorphic_v<MyTest const *>);

    EXPECT_TRUE(instanceOf<A_ const>(a));
    EXPECT_TRUE(instanceOf<A_ const>(aConst));
    EXPECT_TRUE(instanceOf<A_ const>(b));
    EXPECT_TRUE(instanceOf<A_ const>(bConst));
    EXPECT_TRUE(instanceOf<A_ const>(c));
    EXPECT_TRUE(instanceOf<A_ const>(cConst));
    EXPECT_FALSE(instanceOf<A_ const>(e));

    EXPECT_TRUE(instanceOf<B_ const>(a));
    EXPECT_TRUE(instanceOf<B_ const>(aConst));
    EXPECT_TRUE(instanceOf<B_ const>(b));
    EXPECT_TRUE(instanceOf<B_ const>(bConst));
    EXPECT_FALSE(instanceOf<B_ const>(c));
    EXPECT_FALSE(instanceOf<B_ const>(cConst));
    EXPECT_FALSE(instanceOf<B_ const>(e));

    EXPECT_FALSE(instanceOf<MyTest const>(a));
    EXPECT_FALSE(instanceOf<MyTest const>(aConst));
    EXPECT_FALSE(instanceOf<MyTest const>(b));
    EXPECT_FALSE(instanceOf<MyTest const>(bConst));
    EXPECT_FALSE(instanceOf<MyTest const>(c));
    EXPECT_FALSE(instanceOf<MyTest const>(cConst));
    EXPECT_TRUE(instanceOf<MyTest const>(e));
    EXPECT_TRUE(instanceOf<MyTest const>(&e));
    EXPECT_TRUE(instanceOf<MyTest const>(f));
    EXPECT_TRUE(instanceOf<MyTest const>(&f));
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

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}