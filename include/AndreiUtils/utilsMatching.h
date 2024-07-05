//
// Created by Andrei on 05.07.24.
//

#pragma once

#include <cassert>
#include <functional>
#include <queue>
#include <vector>

namespace AndreiUtils {
    int maximalMatchingUnweightedUndirected(std::vector<std::vector<int>> const &adjacency,
                                            int nrVerticesU, int nrVerticesV, bool verbose = false);
}
