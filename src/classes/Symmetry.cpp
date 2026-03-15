//
// Created by Andrei on 13.10.22.
//

#include <AndreiUtils/classes/Symmetry.h>
#include <AndreiUtils/classes/RandomNumberGenerator.hpp>

using namespace AndreiUtils;

Symmetry::Symmetry() = default;

bool Symmetry::operator==(Symmetry const &other) const {
    return this->type == other.type && this->axis == other.axis &&
           this->axisDisplacementFromOrigin == other.axisDisplacementFromOrigin && this->range == other.range;
}

Posed Symmetry::getSymmetricTransformation(double rangeValue) const {
    rangeValue = AndreiUtils::clamp(rangeValue, this->range.first, this->range.second);
    Posed randomPose;
    if (this->type == "rotation") {
        // TODO: what about the axisDisplacement?!
        randomPose = Posed(Eigen::Quaterniond(Eigen::AngleAxis<double>(rangeValue, this->axis)),
                           Eigen::Vector3d::Zero());
    } else {
        // TODO: what about the axisDisplacement?!
        randomPose = Posed(qIdentity<double>(), rangeValue * this->axis);
    }
    return randomPose;
}

Posed Symmetry::getSymmetricTransformation() const {
    double randomRange = RandomNumberGenerator<double>(this->range.first, this->range.second).sample();
    return this->getSymmetricTransformation(randomRange);
}

std::vector<Posed> Symmetry::createSymmetricPoses(std::vector<Posed> const &poses, double const &rangeValue) const {
    std::vector<Posed> newPoses;
    auto randomPose = this->getSymmetricTransformation(rangeValue);
    for (auto const &pose: poses) {
        newPoses.push_back(randomPose * pose);
    }
    return newPoses;
}

std::vector<Posed> Symmetry::createSymmetricPoses(std::vector<Posed> const &poses) const {
    std::vector<Posed> newPoses;
    auto randomPose = this->getSymmetricTransformation();
    for (auto const &pose: poses) {
        newPoses.push_back(randomPose * pose);
    }
    return newPoses;
}

Posed Symmetry::createSymmetricPose(Posed const &pose, double const &rangeValue) const {
    return this->getSymmetricTransformation(rangeValue) * pose;
}

Posed Symmetry::createSymmetricPose(Posed const &pose) const {
    return this->getSymmetricTransformation() * pose;
}

bool Symmetry::checkIfOrientationFitsSymmetry(Eigen::Quaterniond const &q, double axisSimilarityThreshold) const {
    Eigen::AngleAxisd qAxisAngle(q);
    double &qAngle = qAxisAngle.angle();  // <- this is always in [0, pi]
    if (AndreiUtils::equal<double>(qAngle, 0)) {
        return true;
    }
    Eigen::Vector3d &qAxis = qAxisAngle.axis();

    bool posCheck = (qAxis - this->axis).norm() <= axisSimilarityThreshold;
    bool negCheck = (qAxis + this->axis).norm() <= axisSimilarityThreshold;
    if (!posCheck && !negCheck) {
        return false;
    }

    if (posCheck) {
        // symmetry axis == qAxis
        return lessEqual(this->range.first, qAngle) && lessEqual(qAngle, this->range.second);
    }

    // symmetry axis == -qAxis
    return lessEqual(Symmetry::pi_2 - this->range.second, qAngle) &&
           lessEqual(qAngle, Symmetry::pi_2 - this->range.first);
}

double const Symmetry::pi_2(2 * M_PI);
