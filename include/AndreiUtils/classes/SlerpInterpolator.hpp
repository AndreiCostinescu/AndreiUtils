//
// Created by Andrei on 02.09.22.
//

#ifndef ANDREIUTILS_SLERPINTERPOLATOR_HPP
#define ANDREIUTILS_SLERPINTERPOLATOR_HPP

#include <Eigen/Dense>
#include <stdexcept>
#include <string>
#include <vector>

namespace AndreiUtils {
    template<class T>
    class SlerpInterpolator {
    public:
        using InterpolationType = Eigen::Quaternion<T>;

        SlerpInterpolator() = default;

        ~SlerpInterpolator() {
            this->clear();
        }

        void compute(InterpolationType const &start, InterpolationType const &end, int steps, bool withStart = true, bool withEnd = true) {
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
            for (int i = withStart; i < stopIndex; i++) {
                this->result[i] = start.slerp(double(i + !withStart) / interpolationSteps, end);
            }
        }

        void clear() {
            this->result.clear();
        }

        std::vector<InterpolationType> &getResult() {
            return this->result;
        }

        [[nodiscard]] std::vector<InterpolationType> const &getResult() const {
            return this->result;
        }

    protected:
        std::vector<InterpolationType> result;
    };
}

#endif //ANDREIUTILS_SLERPINTERPOLATOR_HPP
