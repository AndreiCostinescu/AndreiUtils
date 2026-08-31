/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
