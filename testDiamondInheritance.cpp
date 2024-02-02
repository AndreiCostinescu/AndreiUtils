//
// Created by Andrei on 22.03.23.
//

#include <iostream>

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

int main() {
    cout << "Hello World!" << endl;

    D d(10);
    d.printData(true);

    return 0;
}
