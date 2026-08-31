// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

    T(T const &t) : s(t.s) { cout << "Copy-constructor" << endl; }

    T(T &&t) noexcept : s(std::move(t.s)) { cout << "Move-constructor" << endl; }

    virtual ~T() { cout << "Destructor" << endl; }

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

std::string f(T &t) { return "Reference"; }

std::string f(T const &t) { return "Const Reference"; }

std::string f(T &&t) { return "Universal Reference"; }

std::string f(T *t) { return "Pointer"; }

std::string f2(T &t) { return f(std::forward<T &>(t)); }

std::string f2(T const &t) { return f(std::forward<T const &>(t)); }

std::string f2(T &&t) { return f(std::forward<T &&>(t)); }

std::string f2(T *t) { return f(t); }

AndreiUtils::Pointer<T> createPointer(T &t) { return AndreiUtils::Pointer<T>(std::forward<T &>(t)); }

AndreiUtils::Pointer<T> createPointer(T &&t) { return AndreiUtils::Pointer<T>(std::forward<T &&>(t)); }

AndreiUtils::Pointer<T> createPointer(T *t) { return {t}; }

void testPerfectForwarding() {
    cout << f(T("Hello")) << endl;
    T x("Hello from x");
    cout << f(x) << endl;
    T const y("Hello from y");
    cout << f(y) << endl;
    cout << f2(T("Hello")) << endl;
    cout << f2(x) << endl;
    cout << f2(y) << endl;

    auto tmp = createPointer(x);
    tmp->s += "!";
    cout << x.s << endl;
    tmp = createPointer(static_cast<T>(y));
    tmp->s += "!";
    cout << y.s << endl;
}

int main() {
    cout << "Hello World!" << endl;

    testPerfectForwarding();

    return 0;
}
