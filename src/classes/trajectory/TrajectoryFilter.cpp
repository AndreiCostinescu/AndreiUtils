//
// Created by andrei on 05.08.23.
//

#include <AndreiUtils/classes/trajectory/TrajectoryFilter.h>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

TrajectoryFilter::TrajectoryFilter() = default;

TrajectoryFilter::~TrajectoryFilter() = default;

Trajectory TrajectoryFilter::filter(Trajectory const &trajectory) {
    Trajectory filtered = trajectory;
    this->filterInPlace(filtered);
    return filtered;
}

void TrajectoryFilter::filterInPlace(Trajectory &trajectory) {
    this->filterInPlace(&trajectory);
}

std::shared_ptr<Trajectory> TrajectoryFilter::filter(Trajectory const *trajectory) {
    shared_ptr<Trajectory> filtered = trajectory->clone();
    this->filterInPlace(filtered.get());
    return filtered;
}

std::shared_ptr<Trajectory> TrajectoryFilter::filter(shared_ptr<Trajectory> const &trajectory) {
    shared_ptr<Trajectory> filtered = trajectory->clone();
    this->filterInPlace(filtered.get());
    return filtered;
}

void TrajectoryFilter::filterInPlace(shared_ptr<Trajectory> &trajectory) {
    this->filterInPlace(trajectory.get());
}

void TrajectoryFilter::reset() {}
