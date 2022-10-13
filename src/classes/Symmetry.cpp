//
// Created by Andrei on 13.10.22.
//

#include <AndreiUtils/classes/Symmetry.h>
#include <AndreiUtils/classes/RandomNumberGenerator.hpp>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

Symmetry::Symmetry() : type(), axis(), axisDisplacementFromOrigin(), range() {}

std::vector<AndreiUtils::Posed> Symmetry::createSymmetricPoses(const vector<AndreiUtils::Posed> &poses) const {
    double randomRange = RandomNumberGenerator<double>(this->range.first, this->range.second).sample();
    // randomRange = 0;
    vector<Posed> newPoses;
    Posed randomPose;
    if (this->type == "rotation") {
        // TODO: what about the axisDisplacement?!
        randomPose = Posed(Quaterniond(AngleAxis<double>(randomRange, this->axis)), Vector3d::Zero());
    } else {
        // TODO: what about the axisDisplacement?!
        randomPose = Posed(qIdentity<double>(), randomRange * this->axis);
    }
    for (auto const &pose: poses) {
        newPoses.push_back(randomPose * pose);
    }
    return newPoses;
}

AndreiUtils::Posed Symmetry::createSymmetricPose(const Posed &pose) const {
    double randomRange = RandomNumberGenerator<double>(this->range.first, this->range.second).sample();
    Posed randomPose;
    if (this->type == "rotation") {
        // TODO: what about the axisDisplacement?!
        randomPose = Posed(Quaterniond(AngleAxis<double>(randomRange, this->axis)), Vector3d::Zero());
    } else {
        // TODO: what about the axisDisplacement?!
        randomPose = Posed(qIdentity<double>(), randomRange * this->axis);
    }
    return randomPose * pose;
}
