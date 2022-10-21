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
        LinearInterpolator() = default;

        ~LinearInterpolator() = default;

        static T singleInterpolation(T const &start, T const &end, double const &tau) {
            return start + tau * (end - start);
        }

        LinearInterpolator &compute(T const &start, double const &stepSize, T const &end) {
            this->clear();

            T diff = end - start;
            for (T tau = 0.; tau <= 1.;) {
                // add the interpolated pose
                this->result.push_back(start + tau * diff);
                tau = tau + stepSize;
            }

            return *this;
        }

        LinearInterpolator &compute(T const &start, T const &end, int steps, bool withStart = true,
                                    bool withEnd = true) {
            if (withStart + withEnd > steps) {
                throw std::runtime_error(
                        "Number of steps smaller than the minimum requested: " + std::to_string(withStart + withEnd));
            }

            this->clear();
            this->result.resize(steps);

            if (withStart) {
                this->result.front() = start;
            }
            if (withEnd) {
                this->result.back() = end;
            }

            T increment = (end - start) / (steps + 1 - withStart - withEnd);
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
        LinearInterpolator &compute(T const &start, T const &end, std::vector<double> const &interpolationPoints) {
            this->clear();
            this->result.resize(interpolationPoints.size());

            T diff = end - start;
            for (int i = 0; i < interpolationPoints.size(); i++) {
                this->result[i] = start + interpolationPoints[i] * diff;
            }

            return *this;
        }
    };
}

#endif //ANDREIUTILS_LINEARINTERPOLATOR_HPP
