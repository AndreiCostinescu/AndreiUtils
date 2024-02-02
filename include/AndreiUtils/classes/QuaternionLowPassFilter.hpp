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

        void resetFilterValue() {
            this->resetFilterValue(AndreiUtils::qIdentity<T>());
        }

        void resetFilterValue(Eigen::Quaternion<T> const &q) {
            this->filterValue = q;
        }

        Eigen::Quaternion<T> const &getFilterValue() const {
            return this->filterValue;
        }

        Eigen::Quaternion<T> &getFilterValue() {
            return this->filterValue;
        }

    protected:
        Eigen::Quaternion<T> filterValue;
        double range, low;
    };
}

#endif //ANDREIUTILS_QUATERNIONLOWPASSFILTER_HPP
