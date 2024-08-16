//
// Created by Andrei Costinescu on 05.08.22.
//

#pragma once

#include <random>
#include <type_traits>

namespace AndreiUtils {
    template<typename T>
    struct uniform_distribution_type {
        using Type = typename std::conditional<std::is_integral<T>::value, std::uniform_int_distribution<T>,
                typename std::conditional<std::is_floating_point<T>::value, std::uniform_real_distribution<T>, void>::type>::type;
    };

    // Can not create move or copy constructors because of the rd (random_device) member variable, which prohibits them
    template<typename T>
    class RandomNumberGenerator {
    public:
        RandomNumberGenerator(T minValue, T maxValue) :
                rd(), seed(rd()), engine(seed), distribution(minValue, maxValue) {}

        RandomNumberGenerator(T minValue, T maxValue, std::random_device::result_type seed) :
                seed(seed), rd(), engine(seed), distribution(minValue, maxValue) {}

        virtual ~RandomNumberGenerator() = default;

        T sample() {
            return this->distribution(this->engine);
        }

        void setSeed(std::random_device::result_type newSeed) {
            this->seed = newSeed;
            this->engine.seed(this->seed);
        }

        [[nodiscard]] std::random_device::result_type const &getSeed() const {
            return this->seed;
        }

    protected:
        // Class member initialization order https://en.cppreference.com/w/cpp/language/constructor#Initialization_order
        std::random_device rd;
        std::random_device::result_type seed;
        std::default_random_engine engine;
        // if floating point type: range is [minValue, maxValue); if integral type: range is [minValue, maxValue]
        typename uniform_distribution_type<T>::Type distribution;
    };

    extern RandomNumberGenerator<double> double01Sampler;

    class RandomNumberGeneratorNormal {
    public:
        RandomNumberGeneratorNormal(double mean, double standardDeviation);

        RandomNumberGeneratorNormal(double mean, double standardDeviation, std::random_device::result_type seed);

        virtual ~RandomNumberGeneratorNormal();

        double sample();

        void setSeed(std::random_device::result_type newSeed);

        [[nodiscard]] std::random_device::result_type const &getSeed() const;

    protected:
        // Class member initialization order https://en.cppreference.com/w/cpp/language/constructor#Initialization_order
        std::random_device rd;
        std::random_device::result_type seed;
        std::default_random_engine engine;
        // if floating point type: range is [minValue, maxValue); if integral type: range is [minValue, maxValue]
        std::normal_distribution<double> distribution;
    };

    extern RandomNumberGeneratorNormal normal01Sampler;
}
