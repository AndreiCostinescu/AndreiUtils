//
// Created by Andrei Costinescu on 23.11.22.
//

#include <AndreiUtils/classes/RandomNumberGenerator.hpp>

AndreiUtils::RandomNumberGenerator<double> AndreiUtils::double01Sampler(0, 1);

AndreiUtils::RandomNumberGeneratorNormal::RandomNumberGeneratorNormal(double mean, double standardDeviation) :
        rd(), seed(rd()), engine(seed), distribution(mean, standardDeviation) {}

AndreiUtils::RandomNumberGeneratorNormal::RandomNumberGeneratorNormal(
        double mean, double standardDeviation, std::random_device::result_type seed) :
        seed(seed), rd(), engine(seed), distribution(mean, standardDeviation) {}

AndreiUtils::RandomNumberGeneratorNormal::~RandomNumberGeneratorNormal() = default;

double AndreiUtils::RandomNumberGeneratorNormal::sample() {
    return this->distribution(this->engine);
}

void AndreiUtils::RandomNumberGeneratorNormal::setSeed(std::random_device::result_type newSeed) {
    this->seed = newSeed;
    this->engine.seed(this->seed);
}

std::random_device::result_type const &AndreiUtils::RandomNumberGeneratorNormal::getSeed() const {
    return this->seed;
}

AndreiUtils::RandomNumberGeneratorNormal AndreiUtils::normal01Sampler(0, 1);