//
// Created by Andrei on 03.11.22.
//

#include <AndreiUtils/classes/Pointer.hpp>
#include <AndreiUtils/utils.hpp>
#include <cassert>
#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using namespace std;

class A {
public:
    explicit A(string name = "") : name(std::move(name)) {}

    virtual ~A() {
        cout << "In A's destructor for " << this->name << "!" << endl;
    }

    A(A const &other) = default;

    A(A &&other) noexcept: name(std::move(other.name)) {}

    A &operator=(A const &other) {
        if (this != &other) {
            this->name = other.name;
        }
        return *this;
    }

    A &operator=(A &&other) noexcept {
        if (this != &other) {
            this->name = std::move(other.name);
        }
        return *this;
    }

    string name;
};

void print(A const *a) {
    cout << a->name << endl;
}

void testFirstExample() {
    std::unique_ptr<A>(new A("1"));
    cout << "After temporary object..." << endl;
    std::unique_ptr<A> a(new A("2"));
    cout << "After a object..." << endl;
    a.reset(new A("3"));
    cout << "After a reset..." << endl;

    print(make_shared<A>("213").get());
    A b("4");
    print(&b);
}

void testResetPointer() {
    auto c = make_shared<A>("1234");
    cout << c.use_count() << endl;
    auto d = c;
    cout << c.use_count() << endl;
    cout << d.use_count() << endl;

    d.reset();
    cout << c.use_count() << endl;
    cout << d.use_count() << endl;
}

void testWeakPtr() {
    cout << endl << endl << "Test Weak Ptr:" << endl;
    // initial state
    auto teacher = make_shared<A>("Jack");
    auto coach = teacher;
    weak_ptr<A> wp = teacher;

    cout << coach.use_count() << endl;
    if (auto temp = wp.lock()) {
        cout << coach.use_count() << endl;
        cout << temp->name << endl;
    }
    cout << coach.use_count() << endl;

    // coach is reset
    coach.reset();

    if (!wp.expired()) {
        cout << "The old teacher is still there." << endl;
    }

    // teacher is reset to Rose
    teacher.reset(new A("Rose"));

    if (wp.expired()) {
        cout << "The old teacher is not there." << endl;
    }
    if (!wp.lock()) {
        cout << "The old teacher is still not there." << endl;
    }
    if (wp.lock() == nullptr) {
        cout << "The old teacher is still not there." << endl;
    }
}

void testSharedPtrWithConst() {
    shared_ptr<A const> a = make_shared<A>("123");
    shared_ptr<A const> c = a;
    print(c.get());
    shared_ptr<A> b = make_shared<A>("321");
    c = b;
    print(c.get());
}

void testSharedPtrWithArray() {
    int n = 10;
    auto x = shared_ptr<A[]>(new A[n]);
    for (int i = 0; i < n; i++) {
        x[i].name = to_string(i);
    }
    x.reset();
}

void testVoidSharedPtr() {
    std::shared_ptr<void> x = make_shared<A>("12345");
    x.reset();
}

void testCopyPointer() {
    shared_ptr<int> x = make_shared<int>(5);
    auto y = x;
    *y = 42;

    cout << *x << " vs. " << *y << endl;
}

class B : public A {
public:
    B() : A("B") {}

    B(B const &other) = default;

    B(B &&other) noexcept: A(std::move(other)), x(other.x++) {}

    B &operator=(B const &other) {
        if (this != &other) {
            this->A::operator=(other);
        }
        return *this;
    }

    B &operator=(B &&other) noexcept {
        if (this != &other) {
            this->A::operator=(std::move(other));
            this->x = other.x++;
        }
        return *this;
    }
protected:
    int x = 0;
};

void testInstanceOf() {
    auto a = make_shared<A>("C");
    auto b = make_shared<B>();
    cout << boolalpha;
    cout << AndreiUtils::pointerInstanceOf<A>(a) << endl;
    cout << AndreiUtils::pointerInstanceOf<A>(b) << endl;
    cout << AndreiUtils::pointerInstanceOf<B>(a) << endl;
    cout << AndreiUtils::pointerInstanceOf<B>(b) << endl;
}

class Tmp {
public:
    std::string s;
};

void testMyPointers() {
    int a = 4;
    AndreiUtils::Pointer<int> x(&a);

    Tmp tmp;
    tmp.s = "Name";
    AndreiUtils::Pointer<Tmp> y(&tmp), z;
    y->s = "Name2";
    *y = tmp;
    cout << x.get() << endl;
    cout << *x << endl;
    cout << y->s << endl;

    shared_ptr<int> shX(make_shared<int>(4));
    shared_ptr<Tmp> shY(make_shared<Tmp>());
    shY->s = "Hello Bada$$";
    x = shX;
    cout << x.get() << endl;
    cout << *x << endl;
    y = std::move(shY);
    cout << y->s << endl;
    y = make_shared<Tmp>();
    cout << "\"" << y->s << "\"" << endl;
    z = y;
    y->s = "Strawberry";
    cout << "\"" << z->s << "\"" << endl;

    // Pointer<int, std::shared_ptr<Tmp>> q;  // throws compile-time error because int != Tmp

    // Pointer<float> fl = x;
    shared_ptr<float> shFl;
    reinterpret_pointer_cast<float>(shX);

    cout << (shFl == nullptr) << endl;
    cout << (x == nullptr) << endl;

    // w.reinterpretCast<float>();  // throws compile-time error because a unique_ptr does not have cast methods...
}

void testMyPointersInstanceOf() {
    A a("123");
    B b;
    AndreiUtils::Pointer<B> x(&b);
    AndreiUtils::Pointer<A> y(&a);

    cout << boolalpha;
    cout << AndreiUtils::pointerInstanceOf<A>(x) << endl;
    cout << AndreiUtils::pointerInstanceOf<A>(y) << endl;
    cout << AndreiUtils::pointerInstanceOf<B>(x) << endl;
    cout << AndreiUtils::pointerInstanceOf<B>(y) << endl;
}

void testMyPointersCast() {
    B b;
    AndreiUtils::Pointer<B> x(&b);
    AndreiUtils::Pointer<B> y(b);
    auto xCast = x.dynamicCast<A>();
    auto yCast = y.dynamicCast<A>();
    assert(xCast != nullptr);
    assert(yCast != nullptr);
}

class Test {
public:
    explicit Test(std::string s) : name(std::move(s)) { cout << "Constructor for " << this->name << "!" << endl; }

    Test(Test const &other) : name(other.name) { cout << "Copy Constructor for " << this->name << "!" << endl; }

    Test(Test &&other) noexcept: name(std::move(other.name)) {
        cout << "Move Constructor for " << this->name << "!" << endl;
    }

    Test &operator=(Test const &other) {
        if (this != &other) {
            this->name = other.name;
            cout << "Copy Assignment for " << this->name << "!" << endl;
        }
        return *this;
    }

    Test &operator=(Test &&other) noexcept {
        if (this != &other) {
            this->name = std::move(other.name);
            cout << "Move Assignment" << this->name << "!" << endl;
        }
        return *this;
    }

    ~Test() { cout << "Destructor of " << this->name << "!" << endl; }

    std::string name;
};

void testConstPointers() {
    int x = 4;
    int const y = 4;
    cout << "0)" << endl;
    AndreiUtils::Pointer<int> intPtr1(x);
    cout << "1)" << endl;
    AndreiUtils::Pointer<int> intPtr2(y);
    cout << "2)" << endl;
    AndreiUtils::Pointer<int> intPtr3((int) y);
    cout << "3)" << endl;
    AndreiUtils::Pointer<int const> intPtr4(x);
    cout << "4)" << endl;
    AndreiUtils::Pointer<int const> intPtr5(y);
    cout << "5)" << endl;

    Test t1("t1");
    Test const t2("t2");
    cout << "0)" << endl;
    AndreiUtils::Pointer<Test> ptr1(t1);
    cout << "1)" << endl;
    // AndreiUtils::Pointer<Test> ptr2(t2);  // fails because Pointer<T> does not accept a "T const" value!
    AndreiUtils::Pointer<Test> ptr2((Test) t2);
    cout << "2)" << endl;
    AndreiUtils::Pointer<Test const> ptr3(t1);
    cout << "3)" << endl;
    AndreiUtils::Pointer<Test const> ptr4(t2);
    cout << "4)" << endl;
    AndreiUtils::Pointer<Test> ptr5(Test("test5"));
    cout << "5)" << endl;
    AndreiUtils::Pointer<Test const> ptr6(Test("test6"));
    cout << "6)" << endl;
    assert(ptr6->name == "test6");
    cout << ptr6->name << endl;
    ptr6 = ptr2;
    cout << "7.1)" << endl;
    assert(ptr2->name == "t2");
    cout << "7.2)" << endl;
    assert(ptr6->name == "t2");
    cout << "7.3)" << endl;
    AndreiUtils::Pointer<Test const> ptr7;
    ptr7 = std::move(ptr2);
    cout << "8.1)" << endl;
    assert(ptr7->name == "t2");
    cout << "8.2)" << endl;
    assert(ptr2 == nullptr);
    cout << "8.3)" << endl;
    assert(ptr6 != nullptr);
    assert(ptr6->name.empty());
    cout << "8.4)" << endl;
    AndreiUtils::Pointer<Test const> ptr8(std::move(ptr7));
    AndreiUtils::Pointer<Test const> ptr9(ptr1);
    AndreiUtils::Pointer<Test const> ptr10(std::move(ptr1));
    // AndreiUtils::Pointer<Test> ptr11(std::move(ptr8));  // fails with wrong const-type
}

void testPointerCast() {
    AndreiUtils::Pointer<A> aPtr;
    B b;
    AndreiUtils::Pointer<B> bPtr(&b);
    aPtr = bPtr;
    assert(aPtr->name == "B");

    AndreiUtils::Pointer<A> aPtr2(b);
    assert(aPtr2->name == "B");

    AndreiUtils::Pointer<A> aPtr3 = std::move(bPtr);
}

TEST(SmartPointers, FirstExample) {
    std::unique_ptr<A> a(new A("2"));
    EXPECT_EQ(a->name, "2");

    a.reset(new A("3"));
    EXPECT_EQ(a->name, "3");

    auto sharedA = make_shared<A>("213");
    EXPECT_EQ(sharedA->name, "213");

    A b("4");
    EXPECT_EQ(b.name, "4");
}

TEST(SmartPointers, ResetPointer) {
    auto c = make_shared<A>("1234");
    EXPECT_EQ(c.use_count(), 1);

    auto d = c;
    EXPECT_EQ(c.use_count(), 2);
    EXPECT_EQ(d.use_count(), 2);

    d.reset();
    EXPECT_EQ(c.use_count(), 1);
    EXPECT_EQ(d.use_count(), 0);
}

TEST(SmartPointers, TestWeakPtr) {

    auto teacher = make_shared<A>("Jack");
    auto coach = teacher;
    weak_ptr<A> wp = teacher;

    EXPECT_EQ(coach.use_count(), 2);

    if (auto temp = wp.lock()) {
        EXPECT_EQ(coach.use_count(), 3);
        EXPECT_EQ(temp->name, "Jack");
    }
    EXPECT_EQ(coach.use_count(), 2);

    coach.reset();
    EXPECT_EQ(coach.use_count(), 0);

    EXPECT_FALSE(wp.expired());

    teacher.reset(new A("Rose"));

    EXPECT_TRUE(wp.expired());
    EXPECT_EQ(wp.lock(), nullptr);
}

TEST(SmartPointers, TestSharedPtrWithConst) {
    auto a = make_shared<A>("123");
    EXPECT_EQ(a->name, "123");

    shared_ptr<const A> c = a;
    EXPECT_EQ(c->name, "123");

    auto b = make_shared<A>("321");
    EXPECT_EQ(b->name, "321");
    c = b;
    EXPECT_EQ(c->name, "321");
    EXPECT_EQ(a.use_count(), 1);
    EXPECT_EQ(b.use_count(), 2);
    EXPECT_EQ(c.use_count(), 2);
}

TEST(SmartPointers, TestSharedPtrWithArray) {
    int n = 10;
    auto x = shared_ptr<A[]>(new A[n]);

    for (int i = 0; i < n; i++) {
        x[i].name = to_string(i);
        EXPECT_EQ(x[i].name, to_string(i));
    }

    x.reset();
    ASSERT_EQ(x.use_count(), 0);
}

TEST(SmartPointers, TestVoidSharedPtr) {

    std::shared_ptr<void> x = make_shared<A>("12345");

    x.reset();

    ASSERT_TRUE(x == nullptr);
}

TEST(SmartPointers, PtrCopy) {
    shared_ptr<int> x = make_shared<int>(5);

    auto y = x;
    *y = 42;
    EXPECT_EQ(*x, *y);
}

TEST(SmartPointers, TestInstanceOf) {
    auto a = make_shared<A>("C");
    auto b = make_shared<B>();

    EXPECT_TRUE((AndreiUtils::pointerInstanceOf<A>(a)));
    EXPECT_TRUE(AndreiUtils::pointerInstanceOf<A>(b));
    EXPECT_FALSE(AndreiUtils::pointerInstanceOf<B>(a));
    EXPECT_TRUE(AndreiUtils::pointerInstanceOf<B>(b));
}

TEST(SmartPointers, MyPointer) {
    int a = 4;
    AndreiUtils::Pointer<int> x(&a);

    Tmp tmp;
    tmp.s = "Name";
    AndreiUtils::Pointer<Tmp> y(&tmp), z;
    y->s = "Name2";
    *y = tmp;
    EXPECT_EQ(x.get(), &a);
    EXPECT_EQ(*x, 4);
    EXPECT_EQ(y->s, "Name2");


    shared_ptr<int> shX(make_shared<int>(4));
    shared_ptr<Tmp> shY(make_shared<Tmp>());
    shY->s = "Hello Bada$$";
    x = shX;
    EXPECT_EQ(x.get(), shX.get());
    EXPECT_EQ(*x, 4);

    y = std::move(shY);
    EXPECT_EQ(y->s, "Hello Bada$$");
    y = make_shared<Tmp>();
    EXPECT_EQ(y->s, "");

    z = y;
    y->s = "Strawberry";
    EXPECT_EQ(z->s, "Strawberry");

    shared_ptr<float> shFl;
    reinterpret_pointer_cast<float>(shX);
    EXPECT_EQ(shFl, nullptr);
    EXPECT_FALSE(x == nullptr);
}

TEST(SmartPointers, MyPointerAssignment) {
    AndreiUtils::Pointer<int> x;
    EXPECT_TRUE(x == nullptr);
    x = 5;
    EXPECT_EQ(*x, 5);
    x = std::make_shared<int>(42);
    EXPECT_EQ(*x, 42);
}

TEST(SmartPointers, SubClassPointer) {
    B b;
    AndreiUtils::Pointer<A> x1(&b);
    AndreiUtils::Pointer<A const> y1(&b);
    B const bConst;
    // AndreiUtils::Pointer<A> z1(&bConst);  // not possible
    AndreiUtils::Pointer<A const> w1(&bConst);

    A a;
    AndreiUtils::Pointer<A> x(&a);
    AndreiUtils::Pointer<A const> y2(&a);
    A const aConst;
    // AndreiUtils::Pointer<A> z2(&aConst);  // not possible
    AndreiUtils::Pointer<A const> w2(&aConst);
}

TEST(SmartPointers, TestTypeWithSubTypes) {
    static_assert(std::is_base_of_v<A, A>);
    static_assert(std::is_base_of_v<A, B>);
    static_assert(std::is_base_of_v<B, B>);
    // static_assert(std::is_base_of_v<B, A>);  // these assertions fail
    // static_assert(std::is_base_of_v<A, int>);  // these assertions fail
    // static_assert(std::is_base_of_v<int, int>);  // these assertions fail
    // static_assert(std::is_base_of_v<float, float>);  // these assertions fail
    // static_assert(std::is_base_of_v<double, double>);  // these assertions fail
}

int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    // testFirstExample();
    // testResetPointer();
    // testWeakPtr();
    // testSharedPtrWithConst();
    // testSharedPtrWithArray();
    // testVoidSharedPtr();
    // testCopyPointer();
    // testInstanceOf();
    // testMyPointers();
    // testMyPointersInstanceOf();
    // testMyPointersCast();
    // testConstPointers();
    // testPointerCast();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}