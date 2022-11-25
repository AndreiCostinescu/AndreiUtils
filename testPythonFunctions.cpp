//
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/classes/PythonInterface.h>

using namespace AndreiUtils;
using namespace std;

void testPythonInterface() {
    PythonInterface p("hello_world", {"print_hello", "return_hello"});
    p.callFunction("print_hello");
    auto res = p.callFunction("return_hello");
    cout << res.cast<string>() << endl;
}

int main() {
    cout << "Hello World!" << endl;

    testPythonInterface();

    return 0;
}
