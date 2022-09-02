//
// Created by Andrei on 02.09.22.
//

#ifndef ANDREIUTILS_LINEARINTERPOLATOR_HPP
#define ANDREIUTILS_LINEARINTERPOLATOR_HPP

#include <stdexcept>
#include <string>
#include <vector>

namespace AndreiUtils {
    template<class T>
    class LinearInterpolator {
    public:
        LinearInterpolator() = default;

        ~LinearInterpolator() {
            this->clear();
        }

        void compute(T const &start, T const &end, int steps, bool withStart = true, bool withEnd = true) {
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
        }

        void clear() {
            this->result.clear();
        }

        std::vector<T> &getResult() {
            return this->result;
        }

        [[nodiscard]] std::vector<T> const &getResult() const {
            return this->result;
        }

    protected:
        std::vector<T> result;
    };
}

#endif //ANDREIUTILS_LINEARINTERPOLATOR_HPP
