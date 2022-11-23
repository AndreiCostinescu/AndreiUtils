//
// Created by Andrei on 21.10.22.
//

#ifndef ANDREIUTILS_POSEINTERPOLATOR_HPP
#define ANDREIUTILS_POSEINTERPOLATOR_HPP

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/Interpolator.hpp>
#include <stdexcept>
#include <string>

namespace AndreiUtils {
    template<class T>
    class PoseInterpolator : public Interpolator<DualQuaternion<T>> {
    public:
        using InterpolationType = DualQuaternion<T>;

        PoseInterpolator() = default;

        ~PoseInterpolator() = default;

        static InterpolationType singleInterpolation(InterpolationType const &start, InterpolationType const &end,
                                                     double const &tau) {
            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            return startNormed.sclerp(tau, endNormed);
        }

        PoseInterpolator &compute(InterpolationType const &start, double const &stepSize,
                                  InterpolationType const &end) {
            this->clear();

            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            InterpolationType endToStart = startNormed.dualQuaternionInverse() * endNormed;

            for (double tau = 0.; tau <= 1.;) {
                // add the interpolated pose
                this->result.emplace_back((tau == 0) ? startNormed : startNormed * endToStart.powScrew(tau));
                tau = tau + stepSize;
            }

            return *this;
        }

        PoseInterpolator &compute(InterpolationType const &start, InterpolationType const &end, int steps,
                                  bool withStart = true, bool withEnd = true) {
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

            int stopIndex = steps - withEnd;
            int interpolationSteps = steps - 1 + !withStart + !withEnd;

            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            InterpolationType endToStart = startNormed.dualQuaternionInverse() * endNormed;

            for (int i = withStart; i < stopIndex; i++) {
                double tau = double(i + !withStart) / interpolationSteps;
                this->result[i] = (tau == 0) ? startNormed : startNormed * endToStart.powScrew(tau);
            }

            return *this;
        }

        // interpolationPoints[i] should be \in [0, 1] for interpolation inside interval and not \in for extrapolation
        PoseInterpolator &compute(InterpolationType const &start, InterpolationType const &end,
                                  std::vector<double> const &interpolationPoints) {
            this->clear();
            this->result.resize(interpolationPoints.size());

            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            InterpolationType endToStart = startNormed.dualQuaternionInverse() * endNormed;

            for (int i = 0; i < interpolationPoints.size(); i++) {
                double const &tau = interpolationPoints[i];
                this->result[i] = (tau == 0) ? startNormed : startNormed * endToStart.powScrew(tau);
            }

            return *this;
        }
    };
}

#endif //ANDREIUTILS_POSEINTERPOLATOR_HPP
