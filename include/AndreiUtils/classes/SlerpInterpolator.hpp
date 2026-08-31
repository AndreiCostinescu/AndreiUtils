/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei on 02.09.22.
//

#ifndef ANDREIUTILS_SLERPINTERPOLATOR_HPP
#define ANDREIUTILS_SLERPINTERPOLATOR_HPP

#include <AndreiUtils/classes/Interpolator.hpp>
#include <Eigen/Dense>
#include <stdexcept>
#include <string>

namespace AndreiUtils {
    template<class T>
    class SlerpInterpolator : public Interpolator<Eigen::Quaternion<T>> {
    public:
        using InterpolationType = Eigen::Quaternion<T>;

        SlerpInterpolator() = default;

        ~SlerpInterpolator() = default;

        static InterpolationType singleInterpolation(InterpolationType const &start, InterpolationType const &end,
                                                     double const &tau) {
            return start.slerp(tau, end);
        }

        SlerpInterpolator &compute(InterpolationType const &start, double const &timeStepSize,
                                   InterpolationType const &end) {
            this->clear();

            for (double tau = 0.; tau <= 1.;) {
                // add the interpolated pose
                this->result.emplace_back(start.slerp(tau, end));
                tau = tau + timeStepSize;
            }

            return *this;
        }

        SlerpInterpolator &compute(InterpolationType const &start, InterpolationType const &end, int steps,
                                   bool withStart = true, bool withEnd = true) {
            if (withStart + withEnd > steps) {
                throw std::runtime_error("Number of steps smaller than the minimum requested: " +
                                         std::to_string(withStart + withEnd));
            }

            this->clear();
            this->result.resize(steps);

            if (withStart) {
                this->result.front() = start;
            }
            if (withEnd) {
                this->result.back() = end;
            }

            int stopIndex = steps - withEnd;
            int interpolationSteps = steps - 1 + !withStart + !withEnd;
            for (int i = withStart; i < stopIndex; i++) {
                this->result[i] = start.slerp(double(i + !withStart) / interpolationSteps, end);
            }

            return *this;
        }

        // interpolationPoints[i] should be \in [0, 1] for interpolation inside interval and not \in for extrapolation
        SlerpInterpolator &compute(InterpolationType const &start, InterpolationType const &end,
                                   std::vector<double> const &interpolationPoints) {
            this->clear();
            this->result.resize(interpolationPoints.size());

            for (int i = 0; i < interpolationPoints.size(); i++) {
                this->result[i] = start.slerp(interpolationPoints[i], end);
            }

            return *this;
        }
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_SLERPINTERPOLATOR_HPP
