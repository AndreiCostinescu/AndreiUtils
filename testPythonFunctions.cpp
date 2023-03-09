//
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/classes/PythonInterface.h>

using namespace AndreiUtils;
using namespace std;

PythonInterface python;

void testPythonInterface() {
    PythonInterface p("hello_world", {"print_hello", "return_hello"});
    cout << p.getFunctions().size() << endl;
    p.callFunction("print_hello");
    auto res = p.callFunction("return_hello");
    cout << res.cast<string>() << endl;
    cout << p.getFunctionSize() << endl;

    python.reInitialize("hello_world", {"print_hello", "return_hello"});
}

int main() {
    cout << "Hello World!" << endl;

    testPythonInterface();

    // without cleaning up the python global variable, the program crashes...
    python.cleanup();
    return 0;
}

