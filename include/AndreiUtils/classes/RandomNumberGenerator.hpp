//
// Created by Andrei Costinescu on 05.08.22.
//

#pragma once

#include <memory>
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
        RandomNumberGenerator(T minValue, T maxValue) : rd(), seed(), engine(), distribution(minValue, maxValue) {
            this->seed = this->rd();
            this->engine = std::make_shared<std::default_random_engine>(this->seed);
        }

        RandomNumberGenerator(T minValue, T maxValue, std::random_device::result_type seed) :
                rd(), seed(seed), engine(), distribution(minValue, maxValue) {
            this->engine = std::make_shared<std::default_random_engine>(this->seed);
        }

        virtual ~RandomNumberGenerator() = default;

        T sample() {
            return this->distribution(*this->engine);
        }

        std::random_device::result_type seed;

    protected:
        std::random_device rd;
        std::shared_ptr<std::default_random_engine> engine;
        // if floating point type: range is [minValue, maxValue); if integral type: range is [minValue, maxValue]
        typename uniform_distribution_type<T>::Type distribution;
    };

    extern RandomNumberGenerator<double> double01Sampler;
}
