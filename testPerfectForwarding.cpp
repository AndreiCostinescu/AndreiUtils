//
// Created by Andrei on 24.07.24.
//

#include <AndreiUtils/classes/Pointer.hpp>
#include <iostream>
#include <string>

using namespace std;

class T {
public:
    T() : s() {}

    explicit T(std::string s) : s(std::move(s)) {}

    T(T const &t) : s(t.s) {
        cout << "Copy-constructor" << endl;
    }

    T(T &&t) noexcept: s(std::move(t.s)) {
        cout << "Move-constructor" << endl;
    }

    virtual ~T() {
        cout << "Destructor" << endl;
    }

    T &operator=(T const &other) {
        if (this != &other) {
            this->s = other.s;
        }
        cout << "Copy-assignment!" << endl;
        return *this;
    }

    T &operator=(T &&other) noexcept {
        if (this != &other) {
            this->s = std::move(other.s);
        }
        cout << "Move-assignment!" << endl;
        return *this;
    }

    std::string s;
};

std::string f(T &t) {
    return "Reference";
}

std::string f(T const &t) {
    return "Const Reference";
}

std::string f(T &&t) {
    return "Universal Reference";
}

std::string f(T *t) {
    return "Pointer";
}

std::string f2(T &t) {
    return f(std::forward<T &>(t));
}

std::string f2(T const &t) {
    return f(std::forward<T const &>(t));
}

std::string f2(T &&t) {
    return f(std::forward<T &&>(t));
}

std::string f2(T *t) {
    return f(t);
}

AndreiUtils::Pointer<T> createPointer(T &t) {
    return AndreiUtils::Pointer<T>(std::forward<T &>(t));
}

AndreiUtils::Pointer<T> createPointer(T const &t) {
    return AndreiUtils::Pointer<T>(std::forward<T const &>(t));
}

AndreiUtils::Pointer<T> createPointer(T &&t) {
    return AndreiUtils::Pointer<T>(std::forward<T &&>(t));
}

AndreiUtils::Pointer<T> createPointer(T *t) {
    return AndreiUtils::Pointer<T>(t);
}

void testPerfectForwarding() {
    cout << f(T("Hello")) << endl;
    T x("Hello from x");
    cout << f(x) << endl;
    T const y("Hello from y");
    cout << f(y) << endl;
    cout << f2(T("Hello")) << endl;
    cout << f2(x) << endl;
    cout << f2(y) << endl;

    createPointer(x)->s += "!";
    cout << x.s << endl;
    createPointer(y)->s += "!";
    cout << y.s << endl;
}

int main() {
    cout << "Hello World!" << endl;

    testPerfectForwarding();

    return 0;
}