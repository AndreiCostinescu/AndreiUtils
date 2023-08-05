//
// Created by andrei on 05.08.23.
//

#include <AndreiUtils/classes/trajectory/Trajectory.h>
#include <AndreiUtils/utilsVector.hpp>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

Trajectory::Trajectory(int size) : size(size), poses(), times() {
    if (this->size != 0) {
        this->poses.resize(size);
        this->times.resize(size);
    }
}

Trajectory::Trajectory(vector<Posed> const &poses, vector<double> const &times) :
        poses(poses), times(times), size(poses.size()) {}

Trajectory::Trajectory(vector<Posed> &&poses, vector<double> &&times) :
        poses(std::move(poses)), times(std::move(times)), size() {
    this->size = this->poses.size();
}

void Trajectory::reserveNewSize(size_t newSize) {
    this->poses.resize(newSize);
    this->times.resize(newSize);
}

void Trajectory::addNewData(vector<Posed> const &newPoses, vector<double> const &newTimes) {
    if (this->size + newPoses.size() > this->poses.size()) {
        this->reserveNewSize(this->size + newPoses.size());
    }
    this->size += newPoses.size();
    vectorAppendInPlace(this->poses, newPoses);
    vectorAppendInPlace(this->times, newTimes);
}

void Trajectory::addNewData(vector<Posed> &&newPoses, vector<double> &&newTimes) {
    if (this->size + newPoses.size() > this->poses.size()) {
        this->reserveNewSize(this->size + newPoses.size());
    }
    this->size += newPoses.size();
    vectorMoveAppendInPlace(this->poses, std::move(newPoses));
    vectorMoveAppendInPlace(this->times, std::move(newTimes));
}

std::vector<Posed> const &Trajectory::getPoses() const {
    return this->poses;
}

std::vector<Posed> &Trajectory::getPoses() {
    return this->poses;
}

std::vector<double> const &Trajectory::getTimes() const {
    return this->times;
}

std::vector<double> &Trajectory::getTimes() {
    return this->times;
}

size_t const &Trajectory::getSize() const {
    return this->size;
}
