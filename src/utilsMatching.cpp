//
// Created by Andrei on 05.07.24.
//

#include <AndreiUtils/utilsMatching.h>
#include <iostream>
#include <limits>

using namespace std;

int AndreiUtils::maximalMatchingUnweightedUndirected(
        std::vector<std::vector<int>> const &adjacency, int nrVerticesU, int nrVerticesV, bool verbose) {
    std::vector<int> matchResult;
    return maximalMatchingUnweightedUndirected(adjacency, nrVerticesU, nrVerticesV, matchResult, verbose);
}

int AndreiUtils::maximalMatchingUnweightedUndirected(
        std::vector<std::vector<int>> const &adjacency, int nrVerticesU, int nrVerticesV, std::vector<int> &matchResult,
        bool verbose) {
    assert(adjacency.size() == nrVerticesU);
    // there is no node-index with this index in U because there are nrVerticesU vertices (starting from 0)
    int INF = -1;

    // pairU[u] stores the pair of u in matching where u is a vertex on left side of Bipartite Graph.
    // If u doesn't have any pair, then pairU[u] is NIL
    int *pairU = new int[nrVerticesU + 1];
    // pairV[v] stores the pair of v in matching. If v doesn't have any pair, then pairU[v] is NIL
    int *pairV = new int[nrVerticesV + 1];
    // dist[u] stores distance of left side vertices
    // dist[u] is one more than dist[u'] if u is next to u' in augmenting path
    int *dist = new int[nrVerticesU + 1];

    // Returns true if there is an augmenting path, else returns false
    std::function<bool()> bfs = [&]() -> bool {
        std::queue<int> Q; // a queue of nodes from u

        // First layer of vertices (set distance as 0)
        for (int u = 0; u < nrVerticesU; ++u) {
            // If this is a free vertex, add it to queue
            if (pairU[u] == nrVerticesV) {
                // u is not matched: free node
                dist[u] = 0;
                Q.push(u);
            } else {
                // Else set distance as infinite so that this vertex is considered next time
                dist[u] = INF;
            }
        }

        // Initialize distance to NIL as infinite
        dist[nrVerticesU] = INF;

        // Q is going to contain vertices of left side only.
        while (!Q.empty()) {
            // Dequeue a vertex
            int u = Q.front();
            Q.pop();

            // If this node is not NIL and can provide a shorter path to NIL
            if (dist[u] != INF && (dist[nrVerticesU] == INF || dist[u] < dist[nrVerticesU])) {
                // Get all adjacent vertices of the dequeued vertex u
                for (auto const &v: adjacency[u]) {
                    // If the pair of v is not considered so far; (v, pairV[V]) is not yet explored edge.
                    if (dist[pairV[v]] == INF) {
                        // Consider the pair and add it to queue
                        dist[pairV[v]] = dist[u] + 1;
                        Q.push(pairV[v]);
                    }
                }
            }
        }

        // If we could come back to NIL using alternating path of distinct vertices then there is an augmenting path
        return (dist[nrVerticesU] != INF);
    };

    std::function<bool(int const &)> dfs = [&](int const &u) -> bool {
        if (u != nrVerticesU) {
            for (auto const &v: adjacency[u]) {
                // v is adjacent to u

                // Follow the distances set by BFS
                if (dist[u] != INF && dist[pairV[v]] != INF && dist[pairV[v]] == dist[u] + 1) {
                    // If dfs for a pair of v also returns true
                    if (dfs(pairV[v])) {
                        pairV[v] = u;
                        pairU[u] = v;
                        return true;
                    }
                }
            }

            // There is no augmenting path beginning with u.
            dist[u] = INF;
            return false;
        }
        return true;
    };

    // Initialize NIL as the pair of all vertices
    for (int u = 0; u <= nrVerticesU; u++) {
        pairU[u] = nrVerticesV;
    }
    for (int v = 0; v <= nrVerticesV; v++) {
        pairV[v] = nrVerticesU;
    }

    // Initialize result
    int result = 0;

    // Keep updating the result while there is an augmenting path.
    while (bfs()) {
        // Find a free vertex
        for (int u = 0; u < nrVerticesU; ++u) {
            // If current vertex is free and there is an augmenting path from current vertex
            if (pairU[u] == nrVerticesV && dfs(u)) {
                ++result;
            }
        }
    }

    matchResult.resize(nrVerticesU, -1);
    for (int i = 0; i < nrVerticesU; ++i) {
        if (pairU[i] != nrVerticesV) {
            matchResult[i] = pairU[i];
            if (verbose) {
                cout << i << " -> " << pairU[i] << endl;
            }
        }
    }
    if (verbose) {
        for (int i = 0; i < nrVerticesV; ++i) {
            if (pairV[i] != nrVerticesU) {
                cout << i << " <- " << pairV[i] << endl;
            }
        }
    }

    delete[] pairU;
    delete[] pairV;
    delete[] dist;
    return result;
}

double AndreiUtils::maximalMatchingWeightedUndirected(std::vector<std::vector<double>> const &weights, int nrVerticesU,
                                                      int nrVerticesV, bool verbose) {
    std::vector<int> matchResult;
    return maximalMatchingWeightedUndirected(weights, nrVerticesU, nrVerticesV, matchResult, verbose);
}

double AndreiUtils::maximalMatchingWeightedUndirected(std::vector<std::vector<double>> const &weights, int nrVerticesU,
                                                      int nrVerticesV, vector<int> &matchResult, bool verbose) {
    if (weights.size() != nrVerticesU) {
        throw std::runtime_error(
                "Weights vector does not have the size of " + std::to_string(nrVerticesU) + " but instead " +
                std::to_string(weights.size()));
    }
    for (auto const &w: weights) {
        if (w.size() != nrVerticesV) {
            throw std::runtime_error(
                    "Weight sub-vector does not have the size of " + std::to_string(nrVerticesV) + " but instead " +
                    std::to_string(w.size()));
        }
    }
    if (nrVerticesU > nrVerticesV) {
        std::vector<int> matchResultSwitch;
        std::vector<std::vector<double>> weightsSwitched(nrVerticesV, std::vector<double>(nrVerticesU));
        for (int i = 0; i < nrVerticesU; ++i) {
            for (int j = 0; j < nrVerticesV; ++j) {
                weightsSwitched[j][i] = weights[i][j];
            }
        }
        auto res = maximalMatchingWeightedUndirected(weightsSwitched, nrVerticesV, nrVerticesU, matchResultSwitch,
                                                     verbose);
        assert(matchResultSwitch.size() == nrVerticesV);
        // switch the match result indices
        matchResult = std::vector<int>(nrVerticesU, -1);
        for (int i = 0; i < matchResultSwitch.size(); ++i) {
            if (matchResultSwitch[i] >= 0) {
                matchResult[matchResultSwitch[i]] = i;
            }
        }
        return res;
    }

    // algorithm adapted from https://cp-algorithms.com/graph/hungarian-algorithm.html

    double const INF = std::numeric_limits<double>::max();
    // u and v are supposed to have size (nrVerticesU + 1) and (nrVerticesV + 1) respectively
    std::vector<double> u(nrVerticesU + 1), v(nrVerticesV + 1);
    std::vector<int> p(nrVerticesV + 1), way(nrVerticesV + 1);
    for (int i = 1; i <= nrVerticesU; ++i) {
        p[0] = i;
        int j0 = 0;
        std::vector<double> minv(nrVerticesV + 1, INF);
        std::vector<bool> used(nrVerticesV + 1, false);
        do {
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            double delta = INF;
            for (int j = 1; j <= nrVerticesV; ++j) {
                if (!used[j]) {
                    double cur = weights[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur, way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j], j1 = j;
                    }
                }
            }
            for (int j = 0; j <= nrVerticesV; ++j) {
                if (used[j]) {
                    if (u[p[j]] < INF / 2 && delta < INF / 2) {
                        u[p[j]] += delta;
                    } else {
                        u[p[j]] = INF;
                    }
                    if (v[j] > -INF / 2 && delta < INF / 2) {
                        v[j] -= delta;
                    } else {
                        v[j] = -INF;
                    }
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);

        // compute/update augmenting path
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }

    if (verbose) {
        cout << "Post matching: " << endl;
        cout << "u = ";
        for (auto const &elem: u) {
            cout << elem << ", ";
        }
        cout << endl;
        cout << "v = ";
        for (auto const &elem: v) {
            cout << elem << ", ";
        }
        cout << endl;
        cout << "p = ";
        for (auto const &elem: p) {
            cout << elem << ", ";
        }
        cout << endl;
        cout << "way = ";
        for (auto const &elem: way) {
            cout << elem << ", ";
        }
        cout << endl;
    }

    matchResult = std::vector<int>(nrVerticesU, -1);
    for (int j = 1; j <= nrVerticesV; ++j) {
        // add to match if weight[match[j]][j] is not infinity!
        if (p[j] > 0 && weights[p[j] - 1][j - 1] != INF) {
            if (verbose) {
                cout << "Creating match " << p[j] - 1 << " to " << j - 1 << endl;
            }
            matchResult[p[j] - 1] = j - 1;
        }
    }

    double cost = -v[0];
    return cost;
}
