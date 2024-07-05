//
// Created by Andrei on 05.07.24.
//

#include <AndreiUtils/utilsMatching.h>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

void testMatching1() {
    std::vector<std::vector<int>> adjacency(4);
    adjacency[0].emplace_back(1);
    adjacency[0].emplace_back(2);
    adjacency[1].emplace_back(0);
    adjacency[2].emplace_back(1);
    adjacency[3].emplace_back(1);
    adjacency[3].emplace_back(3);
    cout << maximalMatchingUnweightedUndirected(adjacency, 4, 4, true) << endl;
}

void testMatching2() {
    int nrU = 4, nrV = 5;
    std::vector<std::vector<int>> adjacency(nrU);
    adjacency[0].emplace_back(0);
    adjacency[0].emplace_back(1);
    adjacency[0].emplace_back(2);
    adjacency[0].emplace_back(3);
    adjacency[0].emplace_back(4);
    adjacency[1].emplace_back(3);
    adjacency[2].emplace_back(1);
    adjacency[3].emplace_back(2);
    adjacency[3].emplace_back(4);
    cout << maximalMatchingUnweightedUndirected(adjacency, nrU, nrV, true) << endl;
}

void testMatching3() {
    int nrU = 4, nrV = 5;
    std::vector<std::vector<int>> adjacency(nrU);
    adjacency[0].emplace_back(0);
    adjacency[0].emplace_back(1);
    adjacency[0].emplace_back(2);
    adjacency[1].emplace_back(3);
    adjacency[1].emplace_back(4);
    adjacency[2].emplace_back(3);
    adjacency[2].emplace_back(4);
    adjacency[3].emplace_back(3);
    adjacency[3].emplace_back(4);
    cout << maximalMatchingUnweightedUndirected(adjacency, nrU, nrV, true) << endl;
}

int main() {
    cout << "Hello World!" << endl;

    testMatching1();
    testMatching2();
    testMatching3();

    return 0;
}