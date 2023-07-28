//
// Created by Andrei on 23.11.22.
//

#ifndef ANDREIUTILS_POSEDECOUPLEDINTERPOLATOR_HPP
#define ANDREIUTILS_POSEDECOUPLEDINTERPOLATOR_HPP

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/Interpolator.hpp>
#include <AndreiUtils/classes/LinearInterpolator.hpp>
#include <AndreiUtils/classes/SlerpInterpolator.hpp>
#include <stdexcept>
#include <string>

namespace AndreiUtils {
    template<class T>
    class PoseDecoupledInterpolator : public Interpolator<DualQuaternion<T>> {
    public:
        using InterpolationType = DualQuaternion<T>;
        using PosType = Eigen::Matrix<T, 3, 1>;
        using RotType = Eigen::Quaternion<T>;

        PoseDecoupledInterpolator() = default;

        ~PoseDecoupledInterpolator() = default;

        static InterpolationType singleInterpolation(InterpolationType const &start, InterpolationType const &end,
                                                     double const &tau) {
            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            return InterpolationType(SlerpInterpolator<T>::singleInterpolation(
                                             startNormed.getRotation(), endNormed.getRotation()),
                                     LinearInterpolator<PosType>::singleInterpolation(
                                             startNormed.getTranslation(), endNormed.getTranslation()));
        }

        PoseDecoupledInterpolator &compute(InterpolationType const &start, double const &timeStepSize,
                                           InterpolationType const &end) {
            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            std::vector<PosType> interpolatedPositions = LinearInterpolator<PosType>().compute(
                    startNormed.getTranslation(), timeStepSize, endNormed.getTranslation()).getResult();
            std::vector<RotType> interpolatedQuaternions = SlerpInterpolator<T>().compute(
                    startNormed.getRotation(), timeStepSize, endNormed.getRotation()).getResult();
            this->setResult(interpolatedPositions.size(), interpolatedQuaternions, interpolatedPositions);
            return *this;
        }

        PoseDecoupledInterpolator &compute(InterpolationType const &start, InterpolationType const &end, int steps,
                                           bool withStart = true, bool withEnd = true) {
            if (withStart + withEnd > steps) {
                throw std::runtime_error(
                        "Number of steps smaller than the minimum requested: " + std::to_string(withStart + withEnd));
            }

            std::vector<PosType> interpolatedPositions = LinearInterpolator<PosType>().compute(
                    start.getTranslation(), end.getTranslation(), steps, withStart, withEnd).getResult();
            std::vector<RotType> interpolatedQuaternions = SlerpInterpolator<T>().compute(
                    start.getRotation().normalized(), end.getRotation().normalized(), steps, withStart,
                    withEnd).getResult();
            this->setResult(steps, interpolatedQuaternions, interpolatedPositions);
            return *this;
        }

        // interpolationPoints[i] should be \in [0, 1] for interpolation inside interval and not \in for extrapolation
        PoseDecoupledInterpolator &compute(InterpolationType const &start, InterpolationType const &end,
                                           std::vector<double> const &interpolationPoints) {
            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            std::vector<PosType> interpolatedPositions = LinearInterpolator<PosType>().compute(
                    startNormed.getTranslation(), endNormed.getTranslation(), interpolationPoints).getResult();
            std::vector<RotType> interpolatedQuaternions = SlerpInterpolator<T>().compute(
                    startNormed.getRotation(), endNormed.getRotation(), interpolationPoints).getResult();
            this->setResult(interpolationPoints.size(), interpolatedQuaternions, interpolatedPositions);
            return *this;
        }

    protected:
        void setResult(size_t const &nrInterpolationPoints, std::vector<RotType> const &interpolatedQuaternions,
                       std::vector<PosType> const &interpolatedPositions) {
            this->clear();
            this->result.resize(nrInterpolationPoints);
            for (size_t i = 0; i < nrInterpolationPoints; i++) {
                this->result[i] = Posed(interpolatedQuaternions[i], interpolatedPositions[i]);
            }
        }
    };
}

#endif //ANDREIUTILS_POSEDECOUPLEDINTERPOLATOR_HPP
