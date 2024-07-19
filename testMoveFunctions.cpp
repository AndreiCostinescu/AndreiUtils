//
// Created by Andrei on 03.11.22.
//

#include <AndreiUtils/classes/MixedDataContainer.hpp>
#include <iostream>
#include <gtest/gtest.h>

using namespace AndreiUtils;
using namespace std;

template<typename T>
class E {
public:
    explicit E(T *data = nullptr) : data(data), ownsData(false) {}

    explicit E(T &&data) : data(new T(std::move(data))), ownsData(true) {}

    explicit E(T &data) = delete;

    E(E const &other) : data(other.data), ownsData(false) {}

    E(E &&other) noexcept: data(other.data), ownsData(other.ownsData) {
        other.ownsData = false;
        other.reset();
    }

    E &operator=(E const &other) {
        if (&other != this) {
            this->discardData();
            this->data = other.data;
            this->ownsData = false;
        }
        return *this;
    }

    E &operator=(E &&other) noexcept {
        if (&other != this) {
            this->discardData();
            this->data = other.data;
            this->ownsData = other.ownsData;
            other.ownsData = false;
            other.reset();
        }
        return *this;
    }

    virtual ~E() {
        this->reset();
    }

    void reset() {
        this->discardData();
        this->ownsData = false;
        this->data = nullptr;
    }

    void discardData() {
        if (this->ownsData) {
            cout << "Call delete!" << endl;
            delete this->data;
            this->data = nullptr;
            this->ownsData = false;
        }
    }

    T* getData() const { return data; }
    bool owns() const { return ownsData; }
    T *data;
    bool ownsData;
};

template<typename T>
void printRValueData(E<T> &&v) {
    cout << "v.data = " << v.data << "; owns it = " << v.ownsData;
    if (v.data != nullptr) {
        cout << "; with value = " << *(v.data);
    }
    cout << endl;
}

template<typename T>
void printRValueData(E<T> &v) = delete;

template<typename T>
void allowOnlyRValues(T &&v) {
    printRValueData(std::forward<T>(v));
    printRValueData(std::forward<T>(v));
}

template<typename T>
void allowOnlyRValues(T &v) = delete;

void testMoveSemantics() {
    E<int> a(42);
    printRValueData(std::move(a));
    cout << endl;
    E<int> b = std::move(a);  // move assignment
    printRValueData(std::move(a));
    printRValueData(std::move(b));
    cout << endl;
    E<int> c(std::move(b));  // move constructor
    printRValueData(std::move(a));
    printRValueData(std::move(b));
    printRValueData(std::move(c));
    cout << endl;
    E<int> d = c;  // copy assignment
    printRValueData(std::move(a));
    printRValueData(std::move(b));
    printRValueData(std::move(c));
    printRValueData(std::move(d));
    cout << endl;
    E<int> e(c);  // copy constructor from c
    printRValueData(std::move(a));
    printRValueData(std::move(b));
    printRValueData(std::move(c));
    printRValueData(std::move(d));
    printRValueData(std::move(e));
    cout << endl;
    E<int> f(d);  // copy constructor from d
    printRValueData(std::move(a));
    printRValueData(std::move(b));
    printRValueData(std::move(c));
    printRValueData(std::move(d));
    printRValueData(std::move(e));
    printRValueData(std::move(f));
    cout << endl;
    allowOnlyRValues(E<int>(69));
}

class F {
public:
    explicit F(int a) : a(a) {}

    F(F const &other) = default;

    F(F &&other) noexcept: a(other.a) {
        other.a = 0;
    }

    F &operator=(F const &other) {
        if (this != &other) {
            this->a = other.a;
        }
        return *this;
    }

    F &operator=(F &&other) noexcept {
        if (this != &other) {
            this->a = other.a;
            other.a = 0;
        }
        return *this;
    }

    int a;
};

void testMixedDataContainerWithNonPointerAdd() {
    MixedDataContainer x;
    F a(42);
    F &b = a;
    F const &f = a;
    std::remove_reference<F &>::type *c = &a;
    x.addData("a", a, true);
    cout << a.a << endl;
    cout << b.a << endl;
    cout << c->a << endl;
    cout << f.a << endl;
    x.addData("b", b, true);
    cout << a.a << endl;
    cout << b.a << endl;
    cout << c->a << endl;
    cout << f.a << endl;
    x.addData("c", c, true);
    cout << a.a << endl;
    cout << b.a << endl;
    cout << c->a << endl;
    cout << f.a << endl;
    x.addData("f", f, true);
    cout << a.a << endl;
    cout << b.a << endl;
    cout << c->a << endl;
    cout << f.a << endl;
    x.addData("d", std::move(a), true);
    cout << a.a << endl;
    cout << b.a << endl;
    cout << c->a << endl;
    cout << f.a << endl;
    x.addData("e", F(3), true);
}

TEST(MoveTest, TestMoveSemantics) {

    E<int> a(42);

    EXPECT_EQ(a.getData() ? *a.getData() : -1, 42);
    EXPECT_TRUE(a.owns());

    E<int> b = std::move(a);
    EXPECT_EQ(a.getData(), nullptr);
    EXPECT_FALSE(a.owns());
    EXPECT_EQ(b.getData() ? *b.getData() : -1, 42);
    EXPECT_TRUE(b.owns());


    E<int> c(std::move(b));
    EXPECT_EQ(b.getData(), nullptr);
    EXPECT_FALSE(b.owns());
    EXPECT_EQ(c.getData() ? *c.getData() : -1, 42);
    EXPECT_TRUE(c.owns());


    E<int> d = c;
    EXPECT_EQ(c.getData() ? *c.getData() : -1, 42);
    EXPECT_TRUE(c.owns());
    EXPECT_EQ(d.getData() ? *d.getData() : -1, 42);
    EXPECT_FALSE(d.owns());


    E<int> e(c);
    EXPECT_EQ(c.getData() ? *c.getData() : -1, 42);
    EXPECT_TRUE(c.owns());
    EXPECT_EQ(e.getData() ? *e.getData() : -1, 42);
    EXPECT_FALSE(e.owns());


    E<int> f(d);
    EXPECT_EQ(d.getData() ? *d.getData() : -1, 42);
    EXPECT_FALSE(d.owns());
    EXPECT_EQ(f.getData() ? *f.getData() : -1, 42);
    EXPECT_FALSE(f.owns());

    E<int> temp(69);
    EXPECT_EQ(temp.getData() ? *temp.getData() : -1, 69);
    EXPECT_TRUE(temp.owns());
}

TEST(MoveTest, TestMixedDataContainerWithNonPointerAdd) {
    MixedDataContainer x;
    F a(42);
    F &b = a;
    F const &f = a;
    std::remove_reference<F &>::type *c = &a;
    x.addData("a", a, true);

    EXPECT_EQ(a.a, 42);
    EXPECT_EQ(b.a, 42);
    EXPECT_EQ(c->a, 42);
    EXPECT_EQ(f.a, 42);

    x.addData("b", b, true);

    EXPECT_EQ(a.a, 42);
    EXPECT_EQ(b.a, 42);
    EXPECT_EQ(c->a, 42);
    EXPECT_EQ(f.a, 42);

    x.addData("c", c, true);
    EXPECT_EQ(a.a, 42);
    EXPECT_EQ(b.a, 42);
    EXPECT_EQ(c->a, 42);
    EXPECT_EQ(f.a, 42);

    x.addData("f", f, true);
    EXPECT_EQ(a.a, 42);
    EXPECT_EQ(b.a, 42);
    EXPECT_EQ(c->a, 42);
    EXPECT_EQ(f.a, 42);

    x.addData("d", std::move(a), true);
    EXPECT_EQ(a.a, 0);
    EXPECT_EQ(b.a, 0);
    EXPECT_EQ(c->a, 0);
    EXPECT_EQ(f.a, 0);
    x.addData("e", F(3), true);
}
int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    // testMoveSemantics();
    // testMixedDataContainerWithNonPointerAdd();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
