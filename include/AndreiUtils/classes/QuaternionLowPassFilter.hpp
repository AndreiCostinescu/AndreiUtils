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
// Created by Andrei on 30.11.22.
//

#ifndef ANDREIUTILS_QUATERNIONLOWPASSFILTER_HPP
#define ANDREIUTILS_QUATERNIONLOWPASSFILTER_HPP

#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsQuaternions.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    template<typename T>
    class QuaternionLowPassFilter {
    public:
        QuaternionLowPassFilter(double range, double center) {
            this->low = AndreiUtils::clamp<double>(center - range / 2, 0, 1);
            auto high = AndreiUtils::clamp<double>(center + range / 2, 0, 1);
            this->range = high - this->low;
        }

        void filter(Eigen::Quaternion<T> const &q) {
            auto tmp = (this->filterValue * q.conjugate()).normalized();
            double dist = 2 * std::acos(AndreiUtils::clamp<double>(tmp.w(), -1, 1));
            double gain = (dist / M_PI) * this->range + this->low;
            this->filterValue = this->filterValue.slerp(gain, q);
        }

        void resetFilterValue() { this->resetFilterValue(AndreiUtils::qIdentity<T>()); }

        void resetFilterValue(Eigen::Quaternion<T> const &q) { this->filterValue = q; }

        Eigen::Quaternion<T> const &getFilterValue() const { return this->filterValue; }

        Eigen::Quaternion<T> &getFilterValue() { return this->filterValue; }

    protected:
        Eigen::Quaternion<T> filterValue;
        double range, low;
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_QUATERNIONLOWPASSFILTER_HPP
