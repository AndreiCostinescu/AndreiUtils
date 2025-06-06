//
// Created by Andrei on 06.06.25.
//

#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsString.h>
#include <AndreiUtils/utilsVector.hpp>

using namespace AndreiUtils;
using namespace std;

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
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("../RobotModelling/123.json")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("../RobotModelling/")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("../RobotModelling")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("./RobotModelling/123.json")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("./RobotModelling/")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("./RobotModelling")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/123.json")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/../RobotModelling2/123.json")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/../RobotModelling2/")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling/../RobotModelling2")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/../RobotModelling2/123.json")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/../RobotModelling2/")) << endl;
    cout << AndreiUtils::simplifyRelativePath(AndreiUtils::getRelativeDirectoryOfPath("/home/../RobotModelling2")) << endl;
    cout << endl;
    cout << AndreiUtils::printVectorToString(splitString(replace(AndreiUtils::getRelativeDirectoryOfPath("/home/andrei/RobotModelling"), "\\", "/"), "/")) << endl;
}

int main() {
    cout << "Hello World!" << endl;

    simplifyPathTest();

    return 0;
}