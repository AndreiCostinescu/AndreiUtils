//
// Created by Andrei on 05.08.23.
//

#include <AndreiUtils/classes/trajectory/Trajectory.h>
#include <AndreiUtils/utilsVector.hpp>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

Trajectory::Trajectory(int size) : size(size), poses(), times(), posesData(make_shared<vector<Posed>>()),
                                   timesData(make_shared<vector<double>>()) {
    this->poses = this->posesData.get();
    this->times = this->timesData.get();
    if (this->size != 0) {
        this->poses->resize(size);
        this->times->resize(size);
    }
}

Trajectory::Trajectory(vector<Posed> const &poses, vector<double> const &times) :
        posesData(make_shared<vector<Posed>>(poses)), timesData(make_shared<vector<double>>(times)),
        size(poses.size()) {
    this->poses = this->posesData.get();
    this->times = this->timesData.get();
}

Trajectory::Trajectory(vector<Posed> &&poses, vector<double> &&times) :
        posesData(make_shared<vector<Posed>>(std::move(poses))),
        timesData(make_shared<vector<double>>(std::move(times))), size() {
    this->poses = this->posesData.get();
    this->times = this->timesData.get();
    this->size = this->poses->size();
}

Trajectory::Trajectory(std::vector<Posed> *poses, std::vector<double> *times) :
        posesData(), timesData(), poses(poses), times(times), size(poses->size()) {}

Trajectory::Trajectory(Trajectory const &other) :
        posesData(other.posesData), timesData(other.timesData), size(other.size), poses(), times() {
    this->updatePointers(other);
}

Trajectory::Trajectory(Trajectory &&other) noexcept:
        posesData(std::move(other.posesData)), timesData(std::move(other.timesData)), size(std::move(other.size)),
        poses(), times() {
    this->updatePointers(std::move(other));
}

Trajectory::~Trajectory() = default;

Trajectory &Trajectory::operator=(Trajectory const &other) {
    if (this != &other) {
        this->size = other.size;
        this->posesData = other.posesData;
        this->timesData = other.timesData;
        this->updatePointers(other);
    }
    return *this;
}

Trajectory &Trajectory::operator=(Trajectory &&other) noexcept {
    if (this != &other) {
        this->size = other.size;
        this->posesData = std::move(other.posesData);
        this->timesData = std::move(other.timesData);
        this->updatePointers(std::move(other));
    }
    return *this;
}

std::shared_ptr<Trajectory> Trajectory::clone() const {
    return make_shared<Trajectory>(*this);
}

void Trajectory::reserveNewSize(size_t newSize) {
    this->poses->resize(newSize);
    this->times->resize(newSize);
}

void Trajectory::addNewData(vector<Posed> const &newPoses, vector<double> const &newTimes) {
    if (this->size + newPoses.size() > this->poses->size()) {
        this->reserveNewSize(this->size + newPoses.size());
    }
    this->size += newPoses.size();
    vectorAppendInPlace(*this->poses, newPoses);
    vectorAppendInPlace(*this->times, newTimes);
}

void Trajectory::addNewData(vector<Posed> &&newPoses, vector<double> &&newTimes) {
    if (this->size + newPoses.size() > this->poses->size()) {
        this->reserveNewSize(this->size + newPoses.size());
    }
    this->size += newPoses.size();
    vectorMoveAppendInPlace(*this->poses, std::move(newPoses));
    vectorMoveAppendInPlace(*this->times, std::move(newTimes));
}

std::vector<Posed> const &Trajectory::getPoses() const {
    return *this->poses;
}

std::vector<Posed> &Trajectory::getPoses() {
    return *this->poses;
}

std::vector<double> const &Trajectory::getTimes() const {
    return *this->times;
}

std::vector<double> &Trajectory::getTimes() {
    return *this->times;
}

size_t const &Trajectory::getSize() const {
    return this->size;
}

void Trajectory::updatePointers(Trajectory const &other) {
    if (this->posesData == nullptr) {
        this->poses = other.poses;
    } else {
        this->poses = this->posesData.get();
    }
    if (this->timesData == nullptr) {
        this->times = other.times;
    } else {
        this->times = this->timesData.get();
    }
}

void Trajectory::updatePointers(Trajectory &&other) {
    if (this->posesData == nullptr) {
        this->poses = std::move(other.poses);
    } else {
        this->poses = this->posesData.get();
    }
    if (this->timesData == nullptr) {
        this->times = std::move(other.times);
    } else {
        this->times = this->timesData.get();
    }
}
