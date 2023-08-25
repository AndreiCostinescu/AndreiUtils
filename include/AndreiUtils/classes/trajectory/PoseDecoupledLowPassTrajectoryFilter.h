//
// Created by Andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/trajectory/TrajectoryFilter.h>
#include <AndreiUtils/classes/QuaternionLowPassFilter.hpp>
#include <AndreiUtils/classes/SlidingWindow.hpp>

namespace AndreiUtils {
    class PoseDecoupledLowPassTrajectoryFilter : public TrajectoryFilter {
    public:
        PoseDecoupledLowPassTrajectoryFilter(int pWindowSize, double qRange, double qCenter);

        explicit PoseDecoupledLowPassTrajectoryFilter(int pWindowSize);

        PoseDecoupledLowPassTrajectoryFilter(double qRange, double qCenter);

        using TrajectoryFilter::filterInPlace;

        void filterInPlace(Trajectory *trajectory) override;

    protected:
        bool filterPosition, filterOrientation;
        std::shared_ptr<QuaternionLowPassFilter<double>> qFilter;
        std::shared_ptr<SlidingWindow<Eigen::Vector3d>> pFilter;
    };
}
