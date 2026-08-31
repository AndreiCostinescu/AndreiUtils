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
// Created by Andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/QuaternionLowPassFilter.hpp>
#include <AndreiUtils/classes/SlidingWindow.hpp>
#include <AndreiUtils/classes/trajectory/CartesianTrajectoryFilter.h>

namespace AndreiUtils {
    class PoseDecoupledLowPassTrajectoryFilter : public CartesianTrajectoryFilter {
    public:
        PoseDecoupledLowPassTrajectoryFilter(int pWindowSize, double qRange, double qCenter);

        explicit PoseDecoupledLowPassTrajectoryFilter(int pWindowSize);

        PoseDecoupledLowPassTrajectoryFilter(double qRange, double qCenter);

        using CartesianTrajectoryFilter::filterInPlace;

        void filterInPlace(CartesianTrajectory *trajectory) override;

    protected:
        bool filterPosition, filterOrientation;
        std::shared_ptr<QuaternionLowPassFilter<double>> qFilter;
        std::shared_ptr<SlidingWindow<Eigen::Vector3d>> pFilter;
    };
} // namespace AndreiUtils
