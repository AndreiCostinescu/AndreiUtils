//
// Created by Andrei on 11.10.22.
//

#include <AndreiUtils/classes/grasp/Grasp.h>
#include <AndreiUtils/utilsGeometry.h>

using namespace AndreiUtils;
using namespace std;

Grasp::Grasp(vector<Posed> const &graspPointPoses, bool hasGraspAngleRange, pair<double, double> const &graspAngleRange)
        : graspPointPoses(graspPointPoses), originalGraspPointPoses(graspPointPoses), graspAngleRange(graspAngleRange),
          hasGraspAngleRange(hasGraspAngleRange) {}

Grasp::~Grasp() = default;

size_t Grasp::getNrPoints() const {
    return this->graspPointPoses.size();
}

vector<Posed> const &Grasp::getGraspPointPoses() const {
    return this->graspPointPoses;
}

void Grasp::setRelativeTo(AndreiUtils::Posed graspRelativeTo) {
    this->relativeTo = std::move(graspRelativeTo);
    this->updateGraspPointPosesFromRelativeTo();
}

void Grasp::composeRelativeTo(AndreiUtils::Posed const &graspRelativeTo) {
    this->relativeTo *= graspRelativeTo;
    this->updateGraspPointPosesFromRelativeTo();
}

pair<double, double> const &Grasp::getGraspAngleRange() const {
    return this->graspAngleRange;
}

pair<double, double> &Grasp::getGraspAngleRange() {
    return this->graspAngleRange;
}

bool Grasp::hasAngleRange() const {
    return this->hasGraspAngleRange;
}

void Grasp::updateGraspPointPosesFromRelativeTo() {
    for (int i = 0; i < this->originalGraspPointPoses.size(); ++i) {
        this->graspPointPoses[i] = this->relativeTo * this->originalGraspPointPoses[i];
    }
}

