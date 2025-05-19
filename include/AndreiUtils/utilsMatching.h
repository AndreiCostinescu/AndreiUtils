//
// Created by Andrei on 05.07.24.
//

#pragma once

#include <cassert>
#include <functional>
#include <queue>
#include <vector>

namespace AndreiUtils {
    int maximalMatchingUnweightedUndirected(
            std::vector<std::vector<int>> const &adjacency, int nrVerticesU, int nrVerticesV, bool verbose = false);

    int maximalMatchingUnweightedUndirected(
            std::vector<std::vector<int>> const &adjacency, int nrVerticesU, int nrVerticesV,
            std::vector<int> &matchResult, bool verbose = false);

    double maximalMatchingMinWeightUndirected(
            std::vector<std::vector<double>> const &weights, int nrVerticesU, int nrVerticesV, bool verbose = false);

    double maximalMatchingMinWeightUndirected(
            std::vector<std::vector<double>> const &weights, int nrVerticesU, int nrVerticesV,
            std::vector<int> &matchResult, bool verbose = false);
}
