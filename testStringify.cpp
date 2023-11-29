//
// Created by Andrei on 29.11.23.
//

#include <AndreiUtils/traits/stringify.hpp>
#include <iostream>

using namespace std;

void testStringifyTypes() {
    int x = 5;
    int &y = x;
    int const &z = x;
    std::complex<int> w(1, 5);
    std::pair<std::complex<double>, bool> v(1.2 + 4.5j, false);
    int *a = &x;
    int *&b = a;
    int *const c = a;
    int *const &d = a;
    int const *e = a;
    int const *&f = e;
    int const *const g = a;
    int const *const &h = a;
    cout << AndreiUtils::toString(x) << endl;
    cout << AndreiUtils::toString(y) << endl;
    cout << AndreiUtils::toString(z) << endl;
    cout << "x address = " << (&x) << endl;
    cout << AndreiUtils::toString(a) << endl;
    cout << AndreiUtils::toString(b) << endl;
    cout << AndreiUtils::toString(c) << endl;
    cout << AndreiUtils::toString(d) << endl;
    cout << AndreiUtils::toString(e) << endl;
    cout << AndreiUtils::toString(f) << endl;
    cout << AndreiUtils::toString(g) << endl;
    cout << AndreiUtils::toString(h) << endl;
    cout << AndreiUtils::toString(w) << endl;
    cout << AndreiUtils::toString(v) << endl;
}

int main() {
    cout << "Hello World!" << endl;

    testStringifyTypes();

    return 0;
}
