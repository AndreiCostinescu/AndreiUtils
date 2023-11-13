//
// Created by Andrei on 05.08.23.
//

#include <AndreiUtils/classes/trajectory/TrajectoryBase.h>
#include <AndreiUtils/utilsVector.hpp>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

TrajectoryBase::TrajectoryBase(int size) : size(size), times(), timesData(make_shared<vector<double>>()) {
    this->times = this->timesData.get();
    if (this->size != 0) {
        this->times->resize(size);
    }
}

TrajectoryBase::TrajectoryBase(vector<double> const &times) : timesData(make_shared<vector<double>>(times)),
                                                              size(times.size()) {
    this->times = this->timesData.get();
}

TrajectoryBase::TrajectoryBase(vector<double> &&times) :
        timesData(make_shared<vector<double>>(std::move(times))), size() {
    this->times = this->timesData.get();
    this->size = this->times->size();
}

TrajectoryBase::TrajectoryBase(std::vector<double> *times) :
        timesData(), times(times), size(times->size()) {}

TrajectoryBase::TrajectoryBase(TrajectoryBase const &other) : timesData(), size(other.size), times() {
    TrajectoryBase::updatePointers(other);
}

TrajectoryBase::TrajectoryBase(TrajectoryBase &&other) noexcept: timesData(), size(std::move(other.size)), times() {
    TrajectoryBase::updatePointers(std::move(other));
}

TrajectoryBase::~TrajectoryBase() = default;

TrajectoryBase &TrajectoryBase::operator=(TrajectoryBase const &other) {
    if (this != &other) {
        this->size = other.size;
        this->updatePointers(other);
    }
    return *this;
}

TrajectoryBase &TrajectoryBase::operator=(TrajectoryBase &&other) noexcept {
    if (this != &other) {
        this->size = other.size;
        this->updatePointers(std::move(other));
    }
    return *this;
}

std::shared_ptr<TrajectoryBase> TrajectoryBase::clone() const {
    return make_shared<TrajectoryBase>(*this);
}

void TrajectoryBase::reserveNewSize(size_t newSize) {
    this->times->reserve(newSize);
}

std::vector<double> const &TrajectoryBase::getTimes() const {
    return *this->times;
}

std::vector<double> &TrajectoryBase::getTimes() {
    return *this->times;
}

size_t const &TrajectoryBase::getSize() const {
    return this->size;
}

void TrajectoryBase::addNewData(vector<double> const &newTimes) {
    if (this->size + newTimes.size() > this->times->size()) {
        this->reserveNewSize(this->size + newTimes.size());
    }
    this->size += newTimes.size();
    vectorAppendInPlace(*this->times, newTimes);
}

void TrajectoryBase::addNewData(vector<double> &&newTimes) {
    if (this->size + newTimes.size() > this->times->size()) {
        this->reserveNewSize(this->size + newTimes.size());
    }
    this->size += newTimes.size();
    vectorMoveAppendInPlace(*this->times, std::move(newTimes));
}

void TrajectoryBase::addNewDatum(double const &newTime) {
    ++this->size;
    this->times->emplace_back(newTime);
}

void TrajectoryBase::addNewDatum(double &&newTime) {
    ++this->size;
    this->times->emplace_back(std::move(newTime));
}

void TrajectoryBase::updatePointers(TrajectoryBase const &other) {
    this->timesData = make_shared<vector<double>>(*other.times);
    this->times = this->timesData.get();
}

void TrajectoryBase::updatePointers(TrajectoryBase &&other) {
    this->timesData = std::move(other.timesData);
    this->times = std::move(other.times);
}
