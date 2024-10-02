//
// Created by Andrei on 02.10.24.
//

#include <iostream>

using namespace std;

class A {
public:
    void f() {
        return this->f(-42);
    }

    virtual void f(int i) {
        cout << "With argument " << i << endl;
    }
};

class B: public A {
public:
    using A::f;

    void f(int i) override {
        cout << "In B: With argument " << i << endl;
    }
};

int main() {
    cout << "Hello World!" << endl;
    B b;
    b.f();
    return 0;
}