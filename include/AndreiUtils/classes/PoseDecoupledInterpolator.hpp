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

        PoseDecoupledInterpolator() = default;

        ~PoseDecoupledInterpolator() = default;

        static InterpolationType singleInterpolation(InterpolationType const &start, InterpolationType const &end,
                                                     double const &tau) {
            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            return InterpolationType(SlerpInterpolator<T>::singleInterpolation(
                                             startNormed.getRotation(), endNormed.getRotation()),
                                     LinearInterpolator<Eigen::Matrix<T, 3, 1>>::singleInterpolation(
                                             startNormed.getTranslation(), endNormed.getTranslation()));
        }

        PoseDecoupledInterpolator &compute(InterpolationType const &start, double const &stepSize,
                                           InterpolationType const &end) {
            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            std::vector<Eigen::Vector3d> interpolatedPositions = LinearInterpolator<Eigen::Matrix<T, 3, 1>>().compute(
                    startNormed.getTranslation(), stepSize, endNormed.getTranslation()).getResult();
            std::vector<Eigen::Quaterniond> interpolatedQuaternions = SlerpInterpolator<T>().compute(
                    startNormed.getRotation(), stepSize, endNormed.getRotation()).getResult();
            this->setResult(interpolatedPositions.size(), interpolatedQuaternions, interpolatedPositions);
            return *this;
        }

        PoseDecoupledInterpolator &compute(InterpolationType const &start, InterpolationType const &end, int steps,
                                           bool withStart = true, bool withEnd = true) {
            if (withStart + withEnd > steps) {
                throw std::runtime_error(
                        "Number of steps smaller than the minimum requested: " + std::to_string(withStart + withEnd));
            }

            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            std::vector<Eigen::Vector3d> interpolatedPositions = LinearInterpolator<Eigen::Matrix<T, 3, 1>>().compute(
                    startNormed.getTranslation(), endNormed.getTranslation(), steps, withStart, withEnd).getResult();
            std::vector<Eigen::Quaterniond> interpolatedQuaternions = SlerpInterpolator<T>().compute(
                    startNormed.getRotation(), endNormed.getRotation(), steps, withStart, withEnd).getResult();
            this->setResult(steps, interpolatedQuaternions, interpolatedPositions);
            return *this;
        }

        // interpolationPoints[i] should be \in [0, 1] for interpolation inside interval and not \in for extrapolation
        PoseDecoupledInterpolator &compute(InterpolationType const &start, InterpolationType const &end,
                                           std::vector<double> const &interpolationPoints) {
            InterpolationType startNormed = start.normalized(), endNormed = end.normalized();
            std::vector<Eigen::Vector3d> interpolatedPositions = LinearInterpolator<Eigen::Matrix<T, 3, 1>>().compute(
                    startNormed.getTranslation(), endNormed.getTranslation(), interpolationPoints).getResult();
            std::vector<Eigen::Quaterniond> interpolatedQuaternions = SlerpInterpolator<T>().compute(
                    startNormed.getRotation(), endNormed.getRotation(), interpolationPoints).getResult();
            this->setResult(interpolationPoints.size(), interpolatedQuaternions, interpolatedPositions);
            return *this;
        }

    protected:
        void setResult(size_t const &nrInterpolationPoints,
                       std::vector<Eigen::Quaternion<T>> const &interpolatedQuaternions,
                       std::vector<Eigen::Matrix<T, 3, 1>> const &interpolatedPositions) {
            this->clear();
            this->result.resize(nrInterpolationPoints);
            for (size_t i = 0; i < nrInterpolationPoints; i++) {
                this->result[i] = Posed(interpolatedQuaternions[i], interpolatedPositions[i]);
            }
        }
    };
}

#endif //ANDREIUTILS_POSEDECOUPLEDINTERPOLATOR_HPP
