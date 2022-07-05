//
// Created by andrei on 23.06.22.
//

#ifndef ANDREIUTILS_DFS_HPP
#define ANDREIUTILS_DFS_HPP

#include <AndreiUtils/classes/graph/Graph.hpp>
#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsVector.hpp>
#include <cassert>

namespace AndreiUtils {
    template<typename NodeId=int, typename EdgeId=std::string>
    class DFS {
        using NodeT = Node<NodeId>;
        using EdgeT = Edge<EdgeId, NodeId>;
        using GraphT = Graph<NodeId, EdgeId>;
    public:
        explicit DFS(GraphT const &_graph, bool recursive = true) : graph(_graph), traversedNodes(0), finishedNodes(0) {
            this->initialize(recursive);
        }

        std::vector<NodeId> getGraphRoots() const {
            return getMapKeys(this->roots);
        }

        std::vector<EdgeId> getTreeEdges() const {
            return getMapKeys(this->treeEdges);
        }

        std::vector<EdgeId> getForwardEdges() const {
            return getMapKeys(this->forwardEdges);
        }

        std::vector<EdgeId> getBackwardEdges() const {
            return getMapKeys(this->backwardEdges);
        }

        std::vector<EdgeId> getCrossEdges() const {
            return getMapKeys(this->crossEdges);
        }

        std::map<NodeId, int> const &getStartNums() const {
            return this->startNum;
        }

        std::map<NodeId, int> &getStartNums() {
            return this->startNum;
        }

        std::map<NodeId, int> const &getFinishNums() const {
            return this->endNum;
        }

        std::map<NodeId, int> &getFinishNums() {
            return this->endNum;
        }

        bool getTopologicalSort(std::vector<NodeId> &container) const {
            if (!this->backwardEdges.empty()) {
                return false;
            }
            container = reverseVector(this->traversal);
            return true;
        }

    protected:
        void initialize(bool recursive = true) {
            for (auto const &x: this->graph.getIncomingEdgesForEachNode()) {
                if (x.second.empty()) {
                    this->roots[x.first] = true;
                }
            }
            if (this->roots.empty()) {
                std::cerr << "Warning: Graph has no roots: will perform a modified DFS version..." << std::endl;
            }
            if (recursive) {
                this->dfsRecursive();
            } else {
                this->dfsIterative();
            }
        }

        void cleanup() {
            this->roots.clear();
            this->crossEdges.clear();
            this->treeEdges.clear();
            this->forwardEdges.clear();
            this->backwardEdges.clear();
            this->startNum.clear();
            this->endNum.clear();
            this->visited.clear();
            this->traversal.clear();
            this->traversedNodes = 0;
            this->finishedNodes = 0;
        }

        void traverseDfsRecursive(NodeId const &nId) {
            this->visited[nId] = true;
            this->startNum[nId] = this->traversedNodes++;
            // node neighbors
            auto neighbors = this->graph.getNeighbors(nId);
            if (neighbors != nullptr) {
                for (auto const &neighbor: *neighbors) {
                    auto neighborId = neighbor.first->getId();
                    auto &edges = this->graph.getEdges(std::make_pair(nId, neighborId));
                    assert (edges.size() >= 1);
                    if (this->edgeClassification(edges, neighborId, nId)) {
                        this->traverseDfsRecursive(neighborId);
                    }
                }
            }
            this->traversal[this->finishedNodes] = nId;
            this->endNum[nId] = this->finishedNodes++;
        }

        void dfsRecursive() {
            this->traversal.resize(this->graph.getNrNodes());
            bool hasRoots = !this->roots.empty();
            std::vector<NodeId> nodeIterationContainer;
            if (hasRoots) {
                nodeIterationContainer = getMapKeys(this->roots);
            } else {
                nodeIterationContainer = getMapKeys(this->graph.getNodes());
            }
            for (auto const &nodeData: nodeIterationContainer) {
                if (!mapContains(this->visited, nodeData)) {
                    this->roots[nodeData] = true;
                    this->traverseDfsRecursive(nodeData);
                }
            }
        }

        void dfsIterative() {
            this->traversal.resize(this->graph.getNrNodes());
            auto &allNeighbors = this->graph.getNeighbors();
            std::map<NodeId, std::tuple<int, int, std::vector<NodeT *>>> neighborIndexAndSize;
            std::vector<NodeId> stack(this->traversal.size());
            std::size_t stackIndex;
            NodeId iterNode, iterNeighborNode;
            bool hasRoots = !this->roots.empty();
            std::vector<NodeId> nodeIterationContainer;
            if (hasRoots) {
                nodeIterationContainer = getMapKeys(this->roots);
            } else {
                nodeIterationContainer = getMapKeys(this->graph.getNodes());
            }
            for (auto const &nodeId: nodeIterationContainer) {
                if (!mapContains(this->visited, nodeId)) {
                    this->roots[nodeId] = true;

                    iterNode = nodeId;
                    this->startNum[iterNode] = this->traversedNodes++;
                    this->visited[iterNode] = true;
                    stack[stackIndex = 0] = iterNode;
                    while (true) {
                        bool found = false;
                        if (!mapContains(neighborIndexAndSize, iterNode)) {
                            std::map<NodeT *, int> const *neighbors;
                            if (mapGetIfContains(allNeighbors, iterNode, neighbors)) {
                                neighborIndexAndSize[iterNode] =
                                        std::make_tuple(0, neighbors->size(), getMapKeys(*neighbors));
                            } else {
                                neighborIndexAndSize[iterNode] = std::make_tuple(0, 0, std::vector<NodeT *>{});
                            }
                        }
                        auto &neighborData = neighborIndexAndSize[iterNode];
                        for (size_t i = std::get<0>(neighborData); i < std::get<1>(neighborData); i++) {
                            std::get<0>(neighborData) = i + 1;
                            iterNeighborNode = std::get<2>(neighborData)[i]->getId();

                            auto &edges = this->graph.getEdges(std::make_pair(iterNode, iterNeighborNode));
                            assert (edges.size() >= 1);
                            if (this->edgeClassification(edges, iterNeighborNode, iterNode)) {
                                found = true;
                                iterNode = iterNeighborNode;
                                this->startNum[iterNode] = this->traversedNodes++;
                                this->visited[iterNode] = true;
                                stack[++stackIndex] = iterNeighborNode;
                                break;
                            }
                        }

                        if (!found) {
                            this->traversal[this->finishedNodes] = iterNode;
                            this->endNum[iterNode] = this->finishedNodes++;
                            if (stackIndex == 0) {
                                break;
                            }
                            stackIndex--;
                            iterNode = stack[stackIndex];
                        }
                    }
                }
            }
        }

        bool edgeClassification(std::map<EdgeT *, bool> const &edges, NodeId const &neighborId,
                                NodeId const &nodeId) {
            bool foundTreeEdge = false;
            int edgeIndex = 0;
            for (auto const &edge: edges) {
                auto edgeId = edge.first->getId();
                if (edgeIndex == 0) {
                    if (!mapContains(this->visited, neighborId)) {
                        // Tree edge
                        this->treeEdges[edgeId] = true;
                        foundTreeEdge = true;
                    } else {
                        if (mapGet(this->startNum, nodeId) > mapGet(this->startNum, neighborId)) {
                            if (mapContains(this->endNum, neighborId)) {
                                // Remove nodes from root-list if present here
                                if (mapContains(this->roots, neighborId)) {
                                    // Tree edge
                                    this->treeEdges[edgeId] = true;
                                    mapDelete(this->roots, neighborId);
                                } else if (mapContains(this->roots, nodeId)) {
                                    // Forward edge
                                    this->forwardEdges[edgeId] = true;
                                } else {
                                    // Cross Edge
                                    this->crossEdges[edgeId] = true;
                                }
                            } else {
                                // Back Edge
                                this->backwardEdges[edgeId] = true;
                            }
                        } else {
                            // Forward edge
                            this->forwardEdges[edgeId] = true;
                        }
                    }
                } else {
                    // Forward edge
                    this->forwardEdges[edgeId] = true;
                }
                ++edgeIndex;
            }
            return foundTreeEdge;
        }

        GraphT const &graph;
        std::map<NodeId, bool> roots;  // nodes that form a root in the DFS-tree
        std::map<EdgeId, bool> crossEdges, treeEdges, forwardEdges, backwardEdges;
        std::map<NodeId, int> startNum, endNum;
        std::map<NodeId, bool> visited;
        std::vector<NodeId> traversal;
        int traversedNodes, finishedNodes;
    };
}

#endif //ANDREIUTILS_DFS_HPP
