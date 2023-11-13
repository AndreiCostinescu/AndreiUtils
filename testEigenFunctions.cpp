//
// Created by Andrei on 25.11.22.
//

#include <AndreiUtils/classes/trajectory/JointTrajectory.hpp>
#include <iostream>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

void testJointTrajectoryClass() {
    JointTrajectory<7> jointTrajectory(10);
    cout << jointTrajectory.getSize() << endl;
    for (auto const &jointValues: jointTrajectory.getJointValues()) {
        cout << jointValues.transpose() << endl;
    }
}

int main() {
    cout << "Hello World!" << endl;

    testJointTrajectoryClass();

    return 0;
}
