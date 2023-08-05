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
