//
// Created by Andrei on 22.03.23.
//

#include <iostream>
#include <gtest/gtest.h>

using namespace std;

class A0 {
public:
    explicit A0(int x) : x(x) { cout << "In constructor of A0" << endl; }

    virtual void printData(bool passOn) const { cout << "Data of A0: " << this->x << endl; }

    int x;
};

class A : public A0 {
public:
    explicit A(int x) : A0(x) { cout << "In constructor of A" << endl; }

    void printData(bool passOn) const override {
        if (passOn) {
            this->A0::printData(false);
        }
        cout << "Data of A: " << this->x << endl;
    }
};

class B : public virtual A {
public:
    explicit B(int x) : A(x) { cout << "In constructor of B: " << this->x << endl; }

    void printData(bool passOn) const override {
        if (passOn) {
            this->A::printData(false);
        }
        cout << "Data of B: " << this->x << endl;
    }
};

class C : public virtual A {
public:
    explicit C(int x) : A(x) { cout << "In constructor of C: " << this->x << endl; }

    void printData(bool passOn) const override {
        if (passOn) {
            this->A::printData(false);
        }
        cout << "Data of C: " << this->x << endl;
    }
};

class D : public virtual B, public virtual C {
public:
    explicit D(int x) : A(x), B(x + 1), C(x + 2) { cout << "In constructor of D" << endl; }

    void printData(bool passOn) const override {
        if (passOn) {
            this->A0::printData(false);  // NOLINT(bugprone-parent-virtual-call)
            this->A::printData(false);  // NOLINT(bugprone-parent-virtual-call)
            this->B::printData(false);
            this->C::printData(false);
        }
        cout << "Data of D: " << this->B::x << endl;
    }
};

TEST(ClassTest, TestConstructorAndPrintData) {
    std::stringstream outputBuffer;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(outputBuffer.rdbuf());

    D d(10);
    d.printData(true);

    std::cout.rdbuf(oldCoutBuffer);

    std::string output = outputBuffer.str();

    std::string expected_output =
        "In constructor of A0\n"
        "In constructor of A\n"
        "In constructor of B: 10\n"
        "In constructor of C: 10\n"
        "In constructor of D\n"
        "Data of A0: 10\n"
        "Data of A: 10\n"
        "Data of B: 10\n"
        "Data of C: 10\n"
        "Data of D: 10\n";

    EXPECT_EQ(output, expected_output);
}

int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    D d(10);
    d.printData(true);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}