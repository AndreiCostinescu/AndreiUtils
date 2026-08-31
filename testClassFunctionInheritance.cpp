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
// Created by Andrei on 02.10.24.
//

#include <iostream>

using namespace std;

class A {
public:
    void f() { return this->f(-42); }

    virtual void f(int i) { cout << "With argument " << i << endl; }
};

class B : public A {
public:
    using A::f;

    void f(int i) override { cout << "In B: With argument " << i << endl; }
};

int main() {
    cout << "Hello World!" << endl;
    B b;
    b.f();
    return 0;
}
