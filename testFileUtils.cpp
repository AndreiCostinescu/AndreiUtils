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
// Created by Andrei on 06.06.25.
//

#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsString.h>
#include <AndreiUtils/utilsVector.hpp>

using namespace AndreiUtils;
using namespace std;

void currentDirectoryTest() { cout << getCurrentDirectory(true) << endl; }

void simplifyPathTest() {
    cout << AndreiUtils::getRelativeDirectoryOfPath("../RobotModelling/123.json") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("../RobotModelling/") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("../RobotModelling") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("./RobotModelling/123.json") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("./RobotModelling/") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("./RobotModelling") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/123.json") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/../RobotModelling2/123.json") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/../RobotModelling2/") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/../RobotModelling2") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("/home/../RobotModelling2/123.json") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("/home/../RobotModelling2/") << endl;
    cout << AndreiUtils::getRelativeDirectoryOfPath("/home/../RobotModelling2") << endl;
    cout << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("../RobotModelling/123.json"))
         << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("../RobotModelling/")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("../RobotModelling")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("./RobotModelling/123.json"))
         << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("./RobotModelling/")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("./RobotModelling")) << endl;
    cout << AndreiUtils::simplifyRelativePath(
                    AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/123.json"))
         << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/"))
         << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling"))
         << endl;
    cout << AndreiUtils::simplifyRelativePath(
                    AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/../RobotModelling2/123.json"))
         << endl;
    cout << AndreiUtils::simplifyRelativePath(
                    AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/../RobotModelling2/"))
         << endl;
    cout << AndreiUtils::simplifyRelativePath(
                    AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/../RobotModelling2"))
         << endl;
    cout << AndreiUtils::simplifyRelativePath(
                    AndreiUtils::getRelativeDirectoryOfPath("/home/../RobotModelling2/123.json"))
         << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/../RobotModelling2/"))
         << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/../RobotModelling2"))
         << endl;
    cout << endl;
    cout << AndreiUtils::printVectorToString(splitString(
                    replace(AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling"), "\\", "/"), "/"))
         << endl;
}

void joinToPathTest() {
    cout << "Join output: " << AndreiUtils::joinToPath({"/home/", "/andrei/", "/relative.txt"}) << endl;
    cout << "Join output: " << AndreiUtils::joinToPath({"/home/", "andrei/", "relative.txt"}) << endl;
}

int main() {
    cout << "Hello World!" << endl;

    currentDirectoryTest();
    simplifyPathTest();
    joinToPathTest();

    return 0;
}
