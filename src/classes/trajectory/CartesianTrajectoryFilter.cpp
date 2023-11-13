//
// Created by Andrei on 05.08.23.
//

#include <AndreiUtils/classes/trajectory/CartesianTrajectoryFilter.h>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

CartesianTrajectoryFilter::CartesianTrajectoryFilter() = default;

CartesianTrajectoryFilter::~CartesianTrajectoryFilter() = default;

CartesianTrajectory CartesianTrajectoryFilter::filter(CartesianTrajectory const &trajectory) {
    CartesianTrajectory filtered = trajectory;
    this->filterInPlace(filtered);
    return filtered;
}

void CartesianTrajectoryFilter::filterInPlace(CartesianTrajectory &trajectory) {
    this->filterInPlace(&trajectory);
}

std::shared_ptr<CartesianTrajectory> CartesianTrajectoryFilter::filter(CartesianTrajectory const *trajectory) {
    shared_ptr<CartesianTrajectory> filtered = dynamic_pointer_cast<CartesianTrajectory>(trajectory->clone());
    this->filterInPlace(filtered.get());
    return filtered;
}

std::shared_ptr<CartesianTrajectory> CartesianTrajectoryFilter::filter(shared_ptr<CartesianTrajectory> const &trajectory) {
    shared_ptr<CartesianTrajectory> filtered = dynamic_pointer_cast<CartesianTrajectory>(trajectory->clone());
    this->filterInPlace(filtered.get());
    return filtered;
}

void CartesianTrajectoryFilter::filterInPlace(shared_ptr<CartesianTrajectory> &trajectory) {
    this->filterInPlace(trajectory.get());
}

void CartesianTrajectoryFilter::reset() {}
