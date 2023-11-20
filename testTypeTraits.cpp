//
// Created by Andrei on 20.11.23.
//

#include <AndreiUtils/traits/is_numeric.hpp>
#include <iostream>

using namespace std;

void testIsNumericType() {
    cout << std::boolalpha;
    cout << "Is integral: " << endl;
    cout << std::is_integral_v<bool> << endl;
    cout << std::is_integral_v<int> << endl;
    cout << std::is_integral_v<double> << endl;
    cout << "Is floating: " << endl;
    cout << std::is_floating_point_v<bool> << endl;
    cout << std::is_floating_point_v<int> << endl;
    cout << std::is_floating_point_v<double> << endl;
    cout << "Is numeric: " << endl;
    cout << AndreiUtils::is_numeric_v<bool> << endl;
    cout << AndreiUtils::is_numeric_v<int> << endl;
    cout << AndreiUtils::is_numeric_v<double> << endl;
}

int main() {
    cout << "Hello World!" << endl;

    testIsNumericType();

    return 0;
}
