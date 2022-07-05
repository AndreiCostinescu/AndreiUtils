//
// Created by andrei on 28.06.22.
//

#ifndef ANDREIUTILS_BFS_HPP
#define ANDREIUTILS_BFS_HPP

#include <AndreiUtils/classes/graph/Graph.hpp>
#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsVector.hpp>
#include <list>

namespace AndreiUtils {
    template<typename NodeId=int, typename EdgeId=std::string>
    class BFS {
        using NodeT = Node<NodeId>;
        using EdgeT = Edge<EdgeId, NodeId>;
        using GraphT = Graph<NodeId, EdgeId>;
    public:
        explicit BFS(GraphT const &graph) : graph(graph), traversalIndex(0) {
            for (auto const &x: this->graph.getIncomingEdgesForEachNode()) {
                if (x.second.empty()) {
                    this->roots[x.first] = true;
                }
            }
            this->traversal.resize(this->graph.getNrNodes());
        }

        std::map<NodeId, std::map<NodeId, int>> bfs() {
            // compute distance from roots to each node that the root reaches
            if (this->roots.empty()) {
                throw std::runtime_error("This graph has no roots! Can not start BFS without any starting nodes");
            }
            std::map<NodeId, std::map<NodeId, int>> res;
            for (auto const &nodeData: this->roots) {
                this->bfsIterative(nodeData.first, nullptr, false);
                res[nodeData.first] = this->distances;
            }
            return res;
        }

        std::map<NodeId, int> bfs(NodeId const &startNode) {
            // compute distance from startNode to each node that the node reaches
            this->bfsIterative(startNode);
            return this->distances;
        }

        int bfs(NodeId const &startNode, NodeId const &endNode) {
            // compute distance from startNode to endNode; finish bfs once endNode is reached
            this->bfsIterative(startNode, &endNode);
            int distance;
            if (mapGetIfContains(this->distances, endNode, distance)) {
                return distance;
            }
            return -1;
        }

        std::vector<NodeId> getGraphRoots() const {
            return getMapKeys(this->roots);
        }

        std::vector<NodeId> getTraversal() const {
            // Starting and Ending iterators
            auto start = this->traversal.begin();
            auto end = this->traversal.begin() + this->traversalIndex;

            std::vector<int> result(this->traversalIndex);  // To store the sliced vector
            copy(start, end, result.begin());  // Copy vector using copy function()
            return result;  // Return the final sliced vector
        }

    protected:
        void bfsIterative(NodeId const &startNode, NodeId const *endNode = nullptr,
                          bool withResetTraversalIndex = true) {
            if (withResetTraversalIndex) {
                this->traversalIndex = 0;
            }

            // Create a queue for BFS
            std::list<NodeId> queue;
            this->distances.clear();

            // Mark the current node as visited and enqueue it
            this->visited[startNode] = true;
            this->distances[startNode] = 0;
            queue.push_back(startNode);

            NodeId iterNodeId;
            int iterNodeDistance;
            while (!queue.empty()) {
                // Dequeue a vertex from queue and print it
                iterNodeId = queue.front();
                iterNodeDistance = this->distances[iterNodeId];
                this->traversal[this->traversalIndex++] = iterNodeId;
                queue.pop_front();
                if (endNode != nullptr && iterNodeId == *endNode) {
                    break;
                }

                auto *neighbors = this->graph.getNeighbors(iterNodeId);
                if (neighbors == nullptr) {
                    continue;
                }
                // Get all adjacent vertices of the dequeued vertex s.
                // If an adjacent has not been visited, then mark it visited and enqueue it
                for (auto const &adjacent: *neighbors) {
                    NodeId &adjacentId = adjacent.first->getId();
                    if (!mapContains(this->visited, adjacentId)) {
                        this->visited[adjacentId] = true;
                        this->distances[adjacentId] = iterNodeDistance + 1;
                        queue.push_back(adjacentId);
                    }
                }
            }
        }

        GraphT const &graph;
        std::map<NodeId, bool> roots;  // nodes that form a root in the BFS-tree
        std::map<NodeId, bool> visited;
        std::vector<NodeId> traversal;
        std::map<NodeId, int> distances;
        int traversalIndex;
    };
}

#endif //ANDREIUTILS_BFS_HPP
