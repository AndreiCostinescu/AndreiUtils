//
// Created by andrei on 23.06.22.
//

#ifndef ANDREIUTILS_DFS_HPP
#define ANDREIUTILS_DFS_HPP

#include <AndreiUtils/classes/graph/Graph.hpp>
#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsVector.hpp>

namespace AndreiUtils {
    template<typename NodeId=int, typename EdgeId=std::string>
    class DFS {
        using NodeT = Node<NodeId>;
        using EdgeT = Edge<EdgeId, NodeId>;
        using GraphT = Graph<NodeId, EdgeId>;
    public:
        DFS(GraphT const &graph, bool recursive) : traversedNodes(0), finishedNodes(0) {
            if (recursive) {
                this->dfsRecursive(graph);
            } else {
                this->dfsIterative(graph);
            }
        }

        explicit DFS(GraphT const &graph) : DFS(graph, true) {}

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

        bool getTopologicalSort(std::vector<NodeId> &container) const {
            if (!this->backwardEdges.empty()) {
                return false;
            }
            container = reverseVector(this->traversal);
            return true;
        }

    protected:
        void traverseDfsRecursive(GraphT const &graph, NodeT const *n) {
            this->visited[n->getId()] = true;
            this->startNum[n->getId()] = this->traversedNodes++;
            // node neighbors
            auto neighbors = graph.getNeighbors(n);
            if (neighbors != nullptr) {
                for (auto const &neighbor: *neighbors) {
                    auto neighborId = neighbor.first->getId();
                    auto &edges = graph.getEdges(std::make_pair(n->getId(), neighborId));
                    assert (edges.size() >= 1);
                    if (this->edgeClassification(graph, edges, neighborId, n->getId())) {
                        this->traverseDfsRecursive(graph, neighbor.first);
                    }
                }
            }
            this->traversal[this->finishedNodes] = n->getId();
            this->endNum[n->getId()] = this->finishedNodes++;
        }

        void dfsRecursive(GraphT const &graph) {
            auto &nodes = graph.getNodes();
            this->traversal.resize(nodes.size());
            for (auto const &nodeData: nodes) {
                if (!mapContains(this->visited, nodeData.first)) {
                    this->roots[nodeData.first] = true;
                    this->traverseDfsRecursive(graph, nodeData.second);
                }
            }
        }

        void dfsIterative(GraphT const &graph) {
            auto &nodes = graph.getNodes();
            this->traversal.resize(nodes.size());
            auto &allNeighbors = graph.getNeighbors();
            std::map<NodeId, std::tuple<int, int, std::vector<NodeT *>>> neighborIndexAndSize;
            std::vector<NodeId> stack(nodes.size());
            std::size_t stackIndex;
            NodeId iterNode, iterNeighborNode;
            for (auto const &nodeData: nodes) {
                if (!mapContains(this->visited, nodeData.first)) {
                    this->roots[nodeData.first] = true;

                    iterNode = nodeData.first;
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

                            auto &edges = graph.getEdges(std::make_pair(iterNode, iterNeighborNode));
                            assert (edges.size() >= 1);
                            if (this->edgeClassification(graph, edges, iterNeighborNode, iterNode)) {
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

        bool edgeClassification(GraphT const &graph, std::map<EdgeT*, bool> const &edges, NodeId const &neighborId, NodeId const &nodeId) {
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

        std::map<NodeId, bool> roots;  // nodes that form a root in the DFS-tree
        std::map<EdgeId, bool> crossEdges, treeEdges, forwardEdges, backwardEdges;
        std::map<NodeId, int> startNum, endNum;
        std::map<NodeId, bool> visited;
        std::vector<NodeId> traversal;
        int traversedNodes, finishedNodes;
    };
}

#endif //ANDREIUTILS_DFS_HPP
