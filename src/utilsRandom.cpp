// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
