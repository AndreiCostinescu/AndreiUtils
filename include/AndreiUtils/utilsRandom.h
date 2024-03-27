//
// Created by Andrei on 22.03.24.
//

#pragma once

#include <vector>

namespace AndreiUtils {
    std::vector<int> createRandomPermutation(int permutationSize);

    std::vector<int> createRandomPermutation(int permutationSize, unsigned long seed);
}