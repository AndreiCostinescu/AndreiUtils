//
// Created by Andrei on 11.10.22.
//

#include <AndreiUtils/classes/grasp/Grasp.h>
#include <AndreiUtils/utilsGeometry.h>
#include "AndreiUtils/classes/grasp/GraspWithAngleRange.h"

using namespace AndreiUtils;
using namespace std;

Grasp::Grasp(vector<Posed> const &graspPointPoses) : graspPointPoses(graspPointPoses),
                                                     originalGraspPointPoses(graspPointPoses) {}

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

void Grasp::updateGraspPointPosesFromRelativeTo() {
    for (int i = 0; i < this->originalGraspPointPoses.size(); ++i) {
        this->graspPointPoses[i] = this->relativeTo * this->originalGraspPointPoses[i];
    }
}

