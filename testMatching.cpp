//
// Created by Andrei on 05.07.24.
//

#include <AndreiUtils/utilsMatching.h>
#include <AndreiUtils/utilsVector.hpp>
#include <iostream>
#include <gtest/gtest.h>

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
    std::vector<int> matchResult;
    cout << maximalMatchingUnweightedUndirected(adjacency, 4, 4, matchResult, true) << endl;
    AndreiUtils::printVector(matchResult);
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
    std::vector<int> matchResult;
    cout << maximalMatchingUnweightedUndirected(adjacency, nrU, nrV, matchResult, true) << endl;
    AndreiUtils::printVector(matchResult);
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
    std::vector<int> matchResult;
    cout << maximalMatchingUnweightedUndirected(adjacency, nrU, nrV, matchResult, true) << endl;
    AndreiUtils::printVector(matchResult);
}

void testWeightedMatching1() {
    int n = 3, m = 4;
    std::vector<std::vector<double>> weights(n, std::vector<double>(m));
    weights[0][0] = 100;
    weights[0][1] = 200;
    weights[0][2] = 300;
    weights[0][3] = 150;
    weights[1][0] = 200;
    weights[1][1] = 400;
    weights[1][2] = 600;
    weights[1][3] = 300;
    weights[2][0] = 450;
    weights[2][1] = 225;
    weights[2][2] = 150;
    weights[2][3] = 300;
    std::vector<int> matchResult;
    double cost = maximalMatchingWeightedUndirected(weights, n, m, matchResult);
    cout << "cost = " << cost << endl;
    cout << "Match: ";
    for (int i = 0; i < matchResult.size(); ++i) {
        cout << "(" << i << " -> " << matchResult[i] << "), ";
    }
    cout << endl;
}

void testWeightedMatching2() {
    int n = 4, m = 3;
    std::vector<std::vector<double>> weights(n, std::vector<double>(m));
    weights[0][0] = 100;
    weights[0][1] = 200;
    weights[0][2] = 450;
    weights[1][0] = 200;
    weights[1][1] = 400;
    weights[1][2] = 225;
    weights[2][0] = 300;
    weights[2][1] = 600;
    weights[2][2] = 150;
    weights[3][0] = 150;
    weights[3][1] = 300;
    weights[3][2] = 300;

    std::vector<int> matchResult;
    double cost = maximalMatchingWeightedUndirected(weights, n, m, matchResult);
    cout << "cost = " << cost << endl;
    cout << "Match: ";
    for (int i = 0; i < matchResult.size(); ++i) {
        cout << "(" << i << " -> " << matchResult[i] << "), ";
    }
    cout << endl;
}

void testWeightedMatching3() {
    int n = 4, m = 3;
    std::vector<std::vector<double>> weights(n, std::vector<double>(m));
    weights[0][0] = 450;
    weights[0][1] = 200;
    weights[0][2] = 100;
    weights[1][0] = 225;
    weights[1][1] = 400;
    weights[1][2] = 200;
    weights[2][0] = 150;
    weights[2][1] = 600;
    weights[2][2] = 300;
    weights[3][0] = 300;
    weights[3][1] = 300;
    weights[3][2] = 150;

    std::vector<int> matchResult;
    double cost = maximalMatchingWeightedUndirected(weights, n, m, matchResult);
    cout << "cost = " << cost << endl;
    cout << "Match: ";
    for (int i = 0; i < matchResult.size(); ++i) {
        cout << "(" << i << " -> " << matchResult[i] << "), ";
    }
    cout << endl;
}

void testWeightedMatching4() {
    int n = 4, m = 3;
    double const INF = std::numeric_limits<double>::max();
    std::vector<std::vector<double>> weights(n, std::vector<double>(m));
    weights[0][0] = 100;
    weights[0][1] = 200;
    weights[0][2] = 450;
    weights[1][0] = INF;
    weights[1][1] = INF;
    weights[1][2] = INF;
    weights[2][0] = 300;
    weights[2][1] = 600;
    weights[2][2] = 150;
    weights[3][0] = 150;
    weights[3][1] = 300;
    weights[3][2] = 300;

    std::vector<int> matchResult;
    double cost = maximalMatchingWeightedUndirected(weights, n, m, matchResult);
    cout << "cost = " << cost << endl;
    cout << "Match: ";
    for (int i = 0; i < matchResult.size(); ++i) {
        cout << "(" << i << " -> " << matchResult[i] << "), ";
    }
    cout << endl;
}

void testWeightedMatching5() {
    int n = 4, m = 3;
    double const INF = std::numeric_limits<double>::max();
    std::vector<std::vector<double>> weights(n, std::vector<double>(m));
    weights[0][0] = 100;
    weights[0][1] = 200;
    weights[0][2] = 450;
    weights[1][0] = INF;
    weights[1][1] = INF;
    weights[1][2] = INF;
    weights[2][0] = INF;
    weights[2][1] = INF;
    weights[2][2] = INF;
    weights[3][0] = 150;
    weights[3][1] = 300;
    weights[3][2] = 300;

    std::vector<int> matchResult;
    double cost = maximalMatchingWeightedUndirected(weights, n, m, matchResult);
    cout << "cost = " << cost << endl;
    cout << "Match: ";
    for (int i = 0; i < matchResult.size(); ++i) {
        cout << "(" << i << " -> " << matchResult[i] << "), ";
    }
    cout << endl;
}

void testWeightedMatching6() {
    int n = 4, m = 3;
    double const INF = std::numeric_limits<double>::max();
    std::vector<std::vector<double>> weights(n, std::vector<double>(m));
    weights[0][0] = 100;
    weights[0][1] = 200;
    weights[0][2] = 450;
    weights[1][0] = 600;
    weights[1][1] = INF;
    weights[1][2] = 900;
    weights[2][0] = 900;
    weights[2][1] = INF;
    weights[2][2] = 600;
    weights[3][0] = 150;
    weights[3][1] = 300;
    weights[3][2] = 300;

    std::vector<int> matchResult;
    double cost = maximalMatchingWeightedUndirected(weights, n, m, matchResult);
    cout << "cost = " << cost << endl;
    cout << "Match: ";
    for (int i = 0; i < matchResult.size(); ++i) {
        cout << "(" << i << " -> " << matchResult[i] << "), ";
    }
    cout << endl;
}

void testWeightedMatching7() {
    int n = 3, m = 3;
    double const INF = std::numeric_limits<double>::max();
    std::vector<std::vector<double>> weights(n, std::vector<double>(m));
    weights[0][0] = 100;
    weights[0][1] = 200;
    weights[0][2] = 450;
    weights[1][0] = 600;
    weights[1][1] = INF;
    weights[1][2] = INF;
    weights[2][0] = 900;
    weights[2][1] = INF;
    weights[2][2] = INF;

    std::vector<int> matchResult;
    double cost = maximalMatchingWeightedUndirected(weights, n, m, matchResult);
    cout << "cost = " << cost << endl;
    cout << "Match: ";
    for (int i = 0; i < matchResult.size(); ++i) {
        cout << "(" << i << " -> " << matchResult[i] << "), ";
    }
    cout << endl;
}

void testWeightedMatching8() {
    int n = 3, m = 3;
    double const INF = std::numeric_limits<double>::max();
    std::vector<std::vector<double>> weights(n, std::vector<double>(m));
    weights[0][0] = 300;
    weights[0][1] = INF;
    weights[0][2] = INF;
    weights[1][0] = 600;
    weights[1][1] = INF;
    weights[1][2] = INF;
    weights[2][0] = 100;
    weights[2][1] = INF;
    weights[2][2] = INF;

    std::vector<int> matchResult;
    double cost = maximalMatchingWeightedUndirected(weights, n, m, matchResult);
    cout << "cost = " << cost << endl;
    cout << "Match: ";
    for (int i = 0; i < matchResult.size(); ++i) {
        cout << "(" << i << " -> " << matchResult[i] << "), ";
    }
    cout << endl;
}

void testWeightedMatching9() {
    int n = 1, m = 3;
    double const INF = std::numeric_limits<double>::max();
    std::vector<std::vector<double>> weights(n, std::vector<double>(m));
    weights[0][0] = INF;
    weights[0][1] = INF;
    weights[0][2] = INF;

    std::vector<int> matchResult;
    double cost = maximalMatchingWeightedUndirected(weights, n, m, matchResult);
    cout << "cost = " << cost << endl;
    cout << "Match: ";
    for (int i = 0; i < matchResult.size(); ++i) {
        cout << "(" << i << " -> " << matchResult[i] << "), ";
    }
    cout << endl;
}

void testWeightedMatching10() {
    int n = 4, m = 3;
    double const INF = std::numeric_limits<double>::max();
    std::vector<std::vector<double>> weights(n, std::vector<double>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            weights[i][j] = INF;
        }
    }

    std::vector<int> matchResult;
    double cost = maximalMatchingWeightedUndirected(weights, n, m, matchResult);
    cout << "cost = " << cost << endl;
    cout << "Match: ";
    for (int i = 0; i < matchResult.size(); ++i) {
        cout << "(" << i << " -> " << matchResult[i] << "), ";
    }
    cout << endl;
}

TEST(MatchingTests, TestMatching1) {
    std::vector<std::vector<int>> adjacency(4);
    adjacency[0].emplace_back(1);
    adjacency[0].emplace_back(2);
    adjacency[1].emplace_back(0);
    adjacency[2].emplace_back(1);
    adjacency[3].emplace_back(1);
    adjacency[3].emplace_back(3);

    int result = AndreiUtils::maximalMatchingUnweightedUndirected(adjacency, 4, 4, false);
    EXPECT_EQ(result, 4);

}

TEST(MatchingTests, TestMatching2) {
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


    int result = AndreiUtils::maximalMatchingUnweightedUndirected(adjacency, 4, 4, false);
    EXPECT_EQ(result, 4);
}
TEST(MatchingTests, TestMatching3) {
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


    int result = AndreiUtils::maximalMatchingUnweightedUndirected(adjacency, 4, 4, false);
    EXPECT_EQ(result, 3);
}

int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    testMatching1();
    testMatching2();
    testMatching3();
    cout << "\n\n";
    testWeightedMatching1();
    testWeightedMatching2();
    testWeightedMatching3();
    testWeightedMatching4();
    testWeightedMatching5();
    testWeightedMatching6();
    testWeightedMatching7();
    testWeightedMatching8();
    testWeightedMatching9();

    double const INF = std::numeric_limits<double>::max();
    cout << -INF << endl;
    cout << -INF - INF << endl;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}