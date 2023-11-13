//
// Created by Andrei on 05.08.23.
//

#include <AndreiUtils/classes/trajectory/CartesianTrajectory.h>
#include <AndreiUtils/utilsVector.hpp>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

CartesianTrajectory::CartesianTrajectory(int size) : TrajectoryBase(size), poses(), posesData(make_shared<vector<Posed>>()) {
    this->poses = this->posesData.get();
    if (this->size != 0) {
        this->poses->resize(size);
    }
}

CartesianTrajectory::CartesianTrajectory(vector<Posed> const &poses, vector<double> const &times) :
        TrajectoryBase(times), posesData(make_shared<vector<Posed>>(poses)) {
    this->poses = this->posesData.get();
}

CartesianTrajectory::CartesianTrajectory(vector<Posed> &&poses, vector<double> &&times) :
        TrajectoryBase(std::move(times)), posesData(make_shared<vector<Posed>>(std::move(poses))) {
    this->poses = this->posesData.get();
}

CartesianTrajectory::CartesianTrajectory(std::vector<Posed> *poses, std::vector<double> *times) :
        TrajectoryBase(times), posesData(), poses(poses) {}

CartesianTrajectory::CartesianTrajectory(CartesianTrajectory const &other) : TrajectoryBase(other), posesData(), poses() {
    CartesianTrajectory::updatePointers(other);
}

CartesianTrajectory::CartesianTrajectory(CartesianTrajectory &&other) noexcept:
        TrajectoryBase(std::move(other)), posesData(), poses() {
    CartesianTrajectory::updatePointers(std::move(other));
}

CartesianTrajectory::~CartesianTrajectory() = default;

CartesianTrajectory &CartesianTrajectory::operator=(CartesianTrajectory const &other) {
    if (this != &other) {
        TrajectoryBase::operator=(other);
        this->CartesianTrajectory::updatePointers(other);
    }
    return *this;
}

CartesianTrajectory &CartesianTrajectory::operator=(CartesianTrajectory &&other) noexcept {
    if (this != &other) {
        TrajectoryBase::operator=(std::move(other));
        this->CartesianTrajectory::updatePointers(std::move(other));
    }
    return *this;
}

std::shared_ptr<TrajectoryBase> CartesianTrajectory::clone() const {
    return make_shared<CartesianTrajectory>(*this);
}

void CartesianTrajectory::reserveNewSize(size_t newSize) {
    TrajectoryBase::reserveNewSize(newSize);
    this->poses->reserve(newSize);
}

void CartesianTrajectory::addNewData(vector<Posed> const &newPoses, vector<double> const &newTimes) {
    TrajectoryBase::addNewData(newTimes);
    vectorAppendInPlace(*this->poses, newPoses);
}

void CartesianTrajectory::addNewData(vector<Posed> &&newPoses, vector<double> &&newTimes) {
    TrajectoryBase::addNewData(std::move(newTimes));
    vectorMoveAppendInPlace(*this->poses, std::move(newPoses));
}

void CartesianTrajectory::addNewDatum(Posed const &newPose, double const &newTime) {
    TrajectoryBase::addNewDatum(newTime);
    this->poses->emplace_back(newPose);
}

void CartesianTrajectory::addNewDatum(Posed &&newPose, double &&newTime) {
    TrajectoryBase::addNewDatum(std::move(newTime));
    this->poses->emplace_back(std::move(newPose));
}

std::vector<Posed> const &CartesianTrajectory::getPoses() const {
    return *this->poses;
}

std::vector<Posed> &CartesianTrajectory::getPoses() {
    return *this->poses;
}

void CartesianTrajectory::updatePointers(CartesianTrajectory const &other) {
    this->posesData = make_shared<vector<Posed>>(*other.poses);
    this->poses = this->posesData.get();
}

void CartesianTrajectory::updatePointers(CartesianTrajectory &&other) {
    this->posesData = std::move(other.posesData);
    this->poses = std::move(other.poses);
}
