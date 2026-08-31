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
// Created by Andrei on 11.10.22.
//

#include <AndreiUtils/classes/grasp/Grasp.h>
#include <AndreiUtils/utilsGeometry.h>

using namespace AndreiUtils;

Grasp::Grasp(std::vector<Posed> const &graspPointPoses, bool hasGraspAngleRange,
             std::pair<double, double> const &graspAngleRange) :
    graspPointPoses(graspPointPoses), originalGraspPointPoses(graspPointPoses), graspAngleRange(graspAngleRange),
    hasGraspAngleRange(hasGraspAngleRange) {}

Grasp::~Grasp() = default;

bool Grasp::operator==(Grasp const &other) const {
    return this->graspPointPoses == other.graspPointPoses && this->hasGraspAngleRange == other.hasGraspAngleRange &&
           (!this->hasAngleRange() || this->graspAngleRange == other.graspAngleRange);
}

size_t Grasp::getNrPoints() const { return this->graspPointPoses.size(); }

void Grasp::update() {}

std::vector<Posed> const &Grasp::getCurrentGraspPointPoses() {
    this->update();
    return this->graspPointPoses;
}

std::vector<Posed> const &Grasp::getGraspPointPoses() const { return this->graspPointPoses; }

void Grasp::setRelativeTo(AndreiUtils::Posed graspRelativeTo) {
    this->relativeTo = std::move(graspRelativeTo);
    this->updateGraspPointPosesFromRelativeTo();
}

void Grasp::composeRelativeTo(AndreiUtils::Posed const &graspRelativeTo) {
    this->relativeTo *= graspRelativeTo;
    this->updateGraspPointPosesFromRelativeTo();
}

std::pair<double, double> const &Grasp::getGraspAngleRange() const { return this->graspAngleRange; }

std::pair<double, double> &Grasp::getGraspAngleRange() { return this->graspAngleRange; }

bool Grasp::hasAngleRange() const { return this->hasGraspAngleRange; }

void Grasp::updateGraspPointPosesFromRelativeTo() {
    for (int i = 0; i < this->originalGraspPointPoses.size(); ++i) {
        this->graspPointPoses[i] = this->relativeTo * this->originalGraspPointPoses[i];
    }
}
