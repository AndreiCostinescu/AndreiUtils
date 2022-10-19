//
// Created by Andrei on 13.10.22.
//

#include <AndreiUtils/classes/Symmetry.h>
#include <AndreiUtils/classes/RandomNumberGenerator.hpp>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

Symmetry::Symmetry() : type(), axis(), axisDisplacementFromOrigin(), range() {}

AndreiUtils::Posed Symmetry::getSymmetricTransformation() const {
    double randomRange = RandomNumberGenerator<double>(this->range.first, this->range.second).sample();
    Posed randomPose;
    if (this->type == "rotation") {
        // TODO: what about the axisDisplacement?!
        randomPose = Posed(Quaterniond(AngleAxis<double>(randomRange, this->axis)), Vector3d::Zero());
    } else {
        // TODO: what about the axisDisplacement?!
        randomPose = Posed(qIdentity<double>(), randomRange * this->axis);
    }
    return randomPose;
}

std::vector<AndreiUtils::Posed> Symmetry::createSymmetricPoses(const vector<AndreiUtils::Posed> &poses) const {
    vector<Posed> newPoses;
    auto randomPose = this->getSymmetricTransformation();
    for (auto const &pose: poses) {
        newPoses.push_back(randomPose * pose);
    }
    return newPoses;
}

AndreiUtils::Posed Symmetry::createSymmetricPose(const Posed &pose) const {
    return this->getSymmetricTransformation() * pose;
}
