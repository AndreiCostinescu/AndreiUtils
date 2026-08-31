/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei Costinescu on 23.11.22.
//

#include <AndreiUtils/classes/RandomNumberGenerator.hpp>

AndreiUtils::RandomNumberGenerator<double> AndreiUtils::double01Sampler(0, 1);

AndreiUtils::RandomNumberGeneratorNormal::RandomNumberGeneratorNormal(double mean, double standardDeviation) :
    rd(), seed(rd()), engine(seed), distribution(mean, standardDeviation) {}

AndreiUtils::RandomNumberGeneratorNormal::RandomNumberGeneratorNormal(double mean, double standardDeviation,
                                                                      std::random_device::result_type seed) :
    seed(seed), rd(), engine(seed), distribution(mean, standardDeviation) {}

AndreiUtils::RandomNumberGeneratorNormal::~RandomNumberGeneratorNormal() = default;

double AndreiUtils::RandomNumberGeneratorNormal::sample() { return this->distribution(this->engine); }

void AndreiUtils::RandomNumberGeneratorNormal::setSeed(std::random_device::result_type newSeed) {
    this->seed = newSeed;
    this->engine.seed(this->seed);
}

std::random_device::result_type const &AndreiUtils::RandomNumberGeneratorNormal::getSeed() const { return this->seed; }

AndreiUtils::RandomNumberGeneratorNormal AndreiUtils::normal01Sampler(0, 1);
