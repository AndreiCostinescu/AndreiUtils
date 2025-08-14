//
// Created by Andrei on 21.08.24.
//

#include <AndreiUtils/classes/ConstexprString.hpp>
#include <AndreiUtils/classes/UserInteraction.hpp>
#include <AndreiUtils/utilsVector.hpp>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

void testMultipleIndices() {
    UserInteraction user(true);
    user << "Enter multiple numbers between 1 and 7";
    cout << "Entered " << printVectorToString(user.getMultipleIndexResponse(1, 7, false)) << endl;

    cout << "Entered " << printVectorToString(user.getMultipleIndexResponse(-1, -7, false)) << endl;
}

void testConstexprString() {
    UserInteraction user(true);
    user << AndreiUtils::concatenate("Enter multiple numbers between 1 and 7");
    cout << "Entered " << printVectorToString(user.getMultipleIndexResponse(1, 7, false)) << endl;
}

int main() {
    cout << "Hello World!" << endl;

    // testMultipleIndices();
    testConstexprString();

    return 0;
}