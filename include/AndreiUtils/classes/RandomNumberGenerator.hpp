//
// Created by Andrei Costinescu on 05.08.22.
//

#ifndef ANDREIUTILS_RANDOMNUMBERGENERATOR_HPP
#define ANDREIUTILS_RANDOMNUMBERGENERATOR_HPP

#include <random>
#include <type_traits>

namespace AndreiUtils {
    template<typename T>
    struct uniform_distribution_type {
        using Type = typename std::conditional<std::is_integral<T>::value, std::uniform_int_distribution<T>,
                typename std::conditional<std::is_floating_point<T>::value, std::uniform_real_distribution<T>, void>::type>::type;
    };

    template<typename T>
    class RandomNumberGenerator {
    public:
        RandomNumberGenerator(T minValue, T maxValue) : rd(), engine(rd()), distribution(minValue, maxValue) {}

        virtual ~RandomNumberGenerator() = default;

        T sample() {
            return this->distribution(this->engine);
        }

    protected:
        std::random_device rd;
        std::default_random_engine engine;
        typename uniform_distribution_type<T>::Type distribution;
    };
}

#endif //ANDREIUTILS_RANDOMNUMBERGENERATOR_HPP
