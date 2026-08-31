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
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/classes/PythonInterface.h>

using namespace AndreiUtils;
using namespace std;

PythonInterface python;

void testPythonInterface() {
    PythonInterface p("scripts.hello_world", {"print_hello", "return_hello"});
    cout << p.getFunctions().size() << endl;
    p.callFunction("print_hello");
    auto res = p.callFunction("return_hello");
    cout << res.cast<string>() << endl;
    cout << p.getFunctionSize() << endl;

    python.reInitialize("scripts.hello_world", {"print_hello", "return_hello"});
}

int main() {
    cout << "Hello World!" << endl;

    testPythonInterface();

    // without cleaning up the python global variable, the program crashes...
    python.cleanup();
    return 0;
}
