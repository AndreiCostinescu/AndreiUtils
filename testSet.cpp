//
// Created by Andrei on 24.11.23.
//

#include <AndreiUtils/utilsSet.hpp>
#include <cassert>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

void testSetCreation() {
    auto s = setFromVector(std::vector<string>{"1", "2", "3", "2"});
    cout << s.size() << endl;
    assert(s.size() == 3);
    printSet(s);

    s = setFromMapKeys(std::map<string, int>{{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}});
    cout << s.size() << endl;
    assert(s.size() == 4);
    printSet(s);
}

int main() {
    cout << "Hello World!" << endl;

    testSetCreation();

    return 0;
}