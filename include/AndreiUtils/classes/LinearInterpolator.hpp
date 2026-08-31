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
// Created by Andrei on 02.09.22.
//

#ifndef ANDREIUTILS_LINEARINTERPOLATOR_HPP
#define ANDREIUTILS_LINEARINTERPOLATOR_HPP

#include <AndreiUtils/classes/Interpolator.hpp>
#include <stdexcept>
#include <string>

namespace AndreiUtils {
    template<class T>
    class LinearInterpolator : public Interpolator<T> {
    public:
        using InterpolationType = T;

        LinearInterpolator() = default;

        ~LinearInterpolator() = default;

        static InterpolationType singleInterpolation(InterpolationType const &start, InterpolationType const &end,
                                                     double const &tau) {
            return start + tau * (end - start);
        }

        LinearInterpolator &compute(InterpolationType const &start, double const &timeStepSize,
                                    InterpolationType const &end) {
            this->clear();

            InterpolationType diff = end - start;
            for (double tau = 0.; tau <= 1.;) {
                // add the interpolated pose
                this->result.emplace_back(start + tau * diff);
                tau = tau + timeStepSize;
            }

            return *this;
        }

        LinearInterpolator &compute(InterpolationType const &start, InterpolationType const &end, int steps,
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

            InterpolationType increment = (end - start) / (steps + 1 - withStart - withEnd);
            int stopIndex = steps - withEnd;
            for (int i = withStart; i < stopIndex; i++) {
                if (i == 0) {
                    this->result[0] = start + increment;
                } else {
                    this->result[i] = this->result[i - 1] + increment;
                }
            }

            return *this;
        }

        // interpolationPoints[i] should be \in [0, 1] for interpolation inside interval and not \in for extrapolation
        LinearInterpolator &compute(InterpolationType const &start, InterpolationType const &end,
                                    std::vector<double> const &interpolationPoints) {
            this->clear();
            this->result.resize(interpolationPoints.size());

            InterpolationType diff = end - start;
            for (int i = 0; i < interpolationPoints.size(); i++) {
                this->result[i] = start + interpolationPoints[i] * diff;
            }

            return *this;
        }
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_LINEARINTERPOLATOR_HPP
