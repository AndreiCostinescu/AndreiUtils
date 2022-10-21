//
// Created by Andrei on 21.10.22.
//

#ifndef ANDREIUTILS_INTERPOLATOR_HPP
#define ANDREIUTILS_INTERPOLATOR_HPP

#include <vector>

namespace AndreiUtils {
    template<class T>
    class Interpolator {
    public:
        Interpolator() = default;

        virtual ~Interpolator() {
            this->clear();
        }

        virtual void clear() {
            this->result.clear();
        }

        [[nodiscard]] std::vector<T> &getResult() {
            return this->result;
        }

        [[nodiscard]] std::vector<T> const &getResult() const {
            return this->result;
        }

    protected:
        std::vector<T> result;
    };
}

#endif //ANDREIUTILS_INTERPOLATOR_HPP
