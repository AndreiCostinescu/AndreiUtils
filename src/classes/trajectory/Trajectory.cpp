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

Trajectory::Trajectory(Trajectory const &other) : posesData(), timesData(), size(other.size), poses(), times() {
    this->updatePointers(other);
}

Trajectory::Trajectory(Trajectory &&other) noexcept:
        posesData(), timesData(), size(std::move(other.size)), poses(), times() {
    this->updatePointers(std::move(other));
}

Trajectory::~Trajectory() = default;

Trajectory &Trajectory::operator=(Trajectory const &other) {
    if (this != &other) {
        this->size = other.size;
        this->updatePointers(other);
    }
    return *this;
}

Trajectory &Trajectory::operator=(Trajectory &&other) noexcept {
    if (this != &other) {
        this->size = other.size;
        this->updatePointers(std::move(other));
    }
    return *this;
}

std::shared_ptr<Trajectory> Trajectory::clone() const {
    return make_shared<Trajectory>(*this);
}

void Trajectory::reserveNewSize(size_t newSize) {
    this->poses->reserve(newSize);
    this->times->reserve(newSize);
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

void Trajectory::addNewDatum(Posed const &newPose, double const &newTime) {
    ++this->size;
    this->poses->emplace_back(newPose);
    this->times->emplace_back(newTime);
}

void Trajectory::addNewDatum(Posed &&newPose, double &&newTime) {
    ++this->size;
    this->poses->emplace_back(std::move(newPose));
    this->times->emplace_back(std::move(newTime));
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
    this->posesData = make_shared<vector<Posed>>(*other.poses);
    this->poses = this->posesData.get();
    this->timesData = make_shared<vector<double>>(*other.times);
    this->times = this->timesData.get();
}

void Trajectory::updatePointers(Trajectory &&other) {
    this->posesData = std::move(other.posesData);
    this->poses = std::move(other.poses);
    this->timesData = std::move(other.timesData);
    this->times = std::move(other.times);
}
