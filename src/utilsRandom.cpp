//
// Created by andrei on 22.03.24.
//

#include <AndreiUtils/utilsRandom.h>
#include <algorithm>
#include <random>

using namespace std;

std::vector<int> AndreiUtils::createRandomPermutation(int permutationSize) {
    vector<int> res(permutationSize);
    for (int i = 0; i < permutationSize; ++i) {
        res[i] = i;
    }
    std::shuffle(res.begin(), res.end(), std::mt19937(std::random_device()()));
    return res;
}

std::vector<int> AndreiUtils::createRandomPermutation(int permutationSize, unsigned long seed) {
    vector<int> res(permutationSize);
    for (int i = 0; i < permutationSize; ++i) {
        res[i] = i;
    }
    std::shuffle(res.begin(), res.end(), std::mt19937(seed));
    return res;
}
