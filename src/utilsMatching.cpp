//
// Created by Andrei on 05.07.24.
//

#include <AndreiUtils/utilsMatching.h>
#include <iostream>

using namespace std;

int AndreiUtils::maximalMatchingUnweightedUndirected(std::vector<std::vector<int>> const &adjacency,
                                                     int nrVerticesU, int nrVerticesV, bool verbose) {
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

    if (verbose) {
        for (int i = 0; i < nrVerticesU; ++i) {
            if (pairU[i] != nrVerticesV) {
                cout << i << " -> " << pairU[i] << endl;
            }
        }
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
