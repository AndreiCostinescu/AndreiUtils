//
// Created by Andrei Costinescu on 23.06.22.
//

#ifndef ANDREIUTILS_GRAPH_HPP
#define ANDREIUTILS_GRAPH_HPP

#include <AndreiUtils/classes/graph/Node.hpp>
#include <AndreiUtils/classes/graph/Edge.hpp>
#include <AndreiUtils/utilsMap.hpp>
#include <cassert>
#include <tuple>

namespace AndreiUtils {
    template<typename NodeId=int, typename EdgeId=std::string>
    class Graph {
        using NodeT = Node<NodeId>;
        using NodeTPtr = std::shared_ptr<NodeT>;
        using EdgeT = Edge<EdgeId, NodeId>;
        using EdgeTPtr = std::shared_ptr<EdgeT>;
        using EdgeIdFunction = std::function<EdgeId(NodeId const &, NodeId const &)>;
    public:
        explicit Graph(bool withMultipleEdges = false, bool withSelfEdges = false) :
                withMultipleEdges(withMultipleEdges), withSelfEdges(withSelfEdges) {}

        Graph(Graph const &other) :
                nodes(other.nodes), edges(other.edges), edgesFromNodeIds(other.edgesFromNodeIds),
                neighbors(other.neighbors), withMultipleEdges(other.withMultipleEdges),
                withSelfEdges(other.withSelfEdges), incomingEdges(other.incomingEdges),
                outgoingEdges(other.outgoingEdges) {}

        Graph(Graph &&other) noexcept:
                nodes(std::move(other.nodes)), edges(std::move(other.edges)),
                edgesFromNodeIds(std::move(other.edgesFromNodeIds)), neighbors(std::move(other.neighbors)),
                withMultipleEdges(std::move(other.withMultipleEdges)), withSelfEdges(std::move(other.withSelfEdges)),
                incomingEdges(std::move(other.incomingEdges)), outgoingEdges(std::move(other.outgoingEdges)) {}

        virtual ~Graph() {
            this->clear();
        }

        Graph &operator=(Graph const &other) noexcept {
            if (&other == this) {
                return *this;
            }
            this->withMultipleEdges = other.withMultipleEdges;
            this->withSelfEdges = other.withSelfEdges;
            this->nodes = other.nodes;
            this->edges = other.edges;
            this->edgesFromNodeIds = other.edgesFromNodeIds;
            this->incomingEdges = other.incomingEdges;
            this->outgoingEdges = other.outgoingEdges;
            this->neighbors = other.neighbors;
            return *this;
        }

        Graph &operator=(Graph &&other) noexcept {
            if (&other == this) {
                return *this;
            }
            this->withMultipleEdges = std::move(other.withMultipleEdges);
            this->withSelfEdges = std::move(other.withSelfEdges);
            this->nodes = std::move(other.nodes);
            this->edges = std::move(other.edges);
            this->edgesFromNodeIds = std::move(other.edgesFromNodeIds);
            this->incomingEdges = std::move(other.incomingEdges);
            this->outgoingEdges = std::move(other.outgoingEdges);
            this->neighbors = std::move(other.neighbors);
            // other.clear();
            return *this;
        }

        void clear() {
            this->withMultipleEdges = false;
            this->withSelfEdges = false;
            this->nodes.clear();
            this->edges.clear();
            this->incomingEdges.clear();
            this->outgoingEdges.clear();
            this->edgesFromNodeIds.clear();
            this->neighbors.clear();
        }

        void addNode(NodeId const &n, std::shared_ptr<NodeData> nodeData = nullptr) {
            this->addNodeImpl(std::make_shared<NodeT>(n, std::move(nodeData)));
        }

        template<class T>
        void addNode(NodeId const &n, std::shared_ptr<T> nodeData) {
            this->addNodeImpl(std::make_shared<NodeT>(n, std::move(nodeData)));
        }

        // the function only accepts r-values as nodeData parameter
        template<class T>
        void addNode(NodeId const &n, T &&nodeData) {
            this->addNodeImpl(std::make_shared<NodeT>(n, std::move(nodeData)));
        }

        // needed for the above to only accept r-values
        template<class T>
        void addNode(NodeId const &n, T &nodeData) = delete;

        // this will allocate new data; only accept r-values
        void addNode(NodeT &&n) {
            this->addNodeImpl(std::make_shared<NodeT>(std::move(n)));
        }

        // needed for the above to only accept r-values
        void addNode(NodeT &n) = delete;

        // this will not allocate new data and any changes within the graph will be reflected on the outside data
        void addNode(NodeTPtr n) {
            this->addNodeImpl(std::move(n));
        }

        void removeNode(NodeId const &nodeId) {
            NodeTPtr const &node = mapGet(this->nodes, nodeId);
            this->removeNode(node);
        }

        void removeNode(NodeT const &node) {
            this->removeNode(node.getId());
        }

        void addEdge(NodeId const &n1, NodeId const &n2, EdgeIdFunction const &createEdgeId,
                     std::shared_ptr<EdgeData> edgeData = nullptr) {
            this->addEdge(EdgeT(mapGet(this->nodes, n1), mapGet(this->nodes, n2),
                                [&](NodeT const &_n1, NodeT const &_n2) {
                                    return createEdgeId(_n1.getId(), _n2.getId());
                                }, std::move(edgeData)));
        }

        template<typename T>
        void addEdge(NodeId const &n1, NodeId const &n2, EdgeIdFunction const &createEdgeId,
                     std::shared_ptr<T> edgeData) {
            this->addEdge(EdgeT(mapGet(this->nodes, n1), mapGet(this->nodes, n2),
                                [&](NodeT const &_n1, NodeT const &_n2) {
                                    return createEdgeId(_n1.getId(), _n2.getId());
                                }, std::move(edgeData)));
        }

        // the function only accepts r-values as edgeData parameter
        template<class T>
        void addEdge(NodeId const &n1, NodeId const &n2, EdgeIdFunction const &createEdgeId, T &&edgeData) {
            this->addEdge(EdgeT(mapGet(this->nodes, n1), mapGet(this->nodes, n2),
                                [&](NodeT const &_n1, NodeT const &_n2) {
                                    return createEdgeId(_n1.getId(), _n2.getId());
                                }, std::move(edgeData)));
        }

        // needed for the above to only accept r-values
        template<class T>
        void addEdge(NodeId const &n1, NodeId const &n2, EdgeIdFunction const &createEdgeId, T &edgeData) = delete;

        // this will allocate new data; only accept r-values
        void addEdge(EdgeT &&e) {
            this->addEdgeImpl(std::make_shared<EdgeT>(std::move(e)));
        }

        // needed for the above to only accept r-values
        void addEdge(EdgeT &e) = delete;

        // this will not allocate new data and any changes within the graph will be reflected on the outside data
        void addEdge(EdgeTPtr e) {
            this->addEdgeImpl(std::move(e));
        }

        // Don't allow setting the edgeData here (if EdgeData derived class has pointers this will create a copy)
        // After adding the undirected edge, add the edge data manually!
        void addUndirectedEdge(NodeId const &n1, NodeId const &n2, EdgeIdFunction const &createEdgeId) {
            auto convert = [&](NodeT const &_n1, NodeT const &_n2) { return createEdgeId(_n1.getId(), _n2.getId()); };
            auto &_n1 = mapGet(this->nodes, n1);
            auto &_n2 = mapGet(this->nodes, n2);
            this->addEdge(EdgeT(_n1, _n2, convert));
            this->addEdge(EdgeT(_n2, _n1, convert));
        }

        void removeEdge(EdgeId const &edgeId) {
            EdgeTPtr const &edge = mapGet(this->edges, edgeId);
            this->removeEdge(edge);
        }

        void removeEdge(EdgeT const &edge) {
            this->removeEdge(edge.getId());
        }

        void removeEdges(NodeId const &n1, NodeId const &n2) {
            for (auto const &edge: mapGet(this->edgesFromNodeIds, std::make_pair(n1, n2))) {
                this->removeEdge(edge.first);
            }
        }

        [[nodiscard]] size_t getNrNodes() const {
            return this->nodes.size();
        }

        [[nodiscard]] size_t getNrEdges() const {
            return this->edges.size();
        }

        [[nodiscard]] std::map<NodeId, NodeTPtr const> &getNodes() {
            return this->nodes;
        }

        [[nodiscard]] std::map<NodeId, NodeTPtr const> const &getNodes() const {
            return this->nodes;
        }

        [[nodiscard]] std::map<EdgeId, EdgeTPtr const> &getEdges() {
            return this->edges;
        }

        [[nodiscard]] std::map<EdgeId, EdgeTPtr const> const &getEdges() const {
            return this->edges;
        }

        [[nodiscard]] bool hasNode(NodeId const &nodeId) const {
            return mapContains(this->nodes, nodeId);
        }

        [[nodiscard]] bool hasEdge(EdgeId const &edgeId) const {
            return mapContains(this->edges, edgeId);
        }

        [[nodiscard]] NodeTPtr const &getNode(NodeId const &nodeId) const {
            return mapGet(this->nodes, nodeId);
        }

        [[nodiscard]] EdgeTPtr const &getEdge(EdgeId const &edgeId) const {
            return mapGet(this->edges, edgeId);
        }

        [[nodiscard]] std::map<EdgeTPtr, bool> const &getEdges(std::pair<NodeId, NodeId> const &edgeId) const {
            return mapGet(this->edgesFromNodeIds, edgeId);
        }

        [[nodiscard]] std::map<NodeId, std::map<NodeTPtr, int>> const &getNeighbors() const {
            return this->neighbors;
        }

        [[nodiscard]] std::map<NodeTPtr, int> const *getNeighbors(NodeTPtr const &n) const {
            return this->getNeighbors(n->getId());
        }

        [[nodiscard]] std::map<NodeTPtr, int> const *getNeighbors(NodeId const &nId) const {
            if (!mapContains(this->neighbors, nId)) {
                return nullptr;
            }
            return &mapGet(this->neighbors, nId);
        }

        [[nodiscard]] std::map<NodeId, std::map<EdgeTPtr, bool>> &getIncomingEdgesForEachNode() {
            return this->incomingEdges;
        }

        [[nodiscard]] std::map<NodeId, std::map<EdgeTPtr, bool>> const &getIncomingEdgesForEachNode() const {
            return this->incomingEdges;
        }

        [[nodiscard]] std::map<NodeId, std::map<EdgeTPtr, bool>> &getOutgoingEdgesForEachNode() {
            return this->outgoingEdges;
        }

        [[nodiscard]] std::map<NodeId, std::map<EdgeTPtr, bool>> const &getOutgoingEdgesForEachNode() const {
            return this->outgoingEdges;
        }

        void changeEdge(EdgeId const &edgeId, EdgeId const &newEdgeId, NodeId const &newN1Id, NodeId const &newN2Id) {
            if (!this->withMultipleEdges && this->hasEdge(newEdgeId)) {
                throw std::runtime_error("Edge to be changed already exists in the graph!");
            }
            auto &edge = mapGet(this->edges, edgeId);
            NodeTPtr const &newN1 = mapGet(this->nodes, newN1Id);
            NodeTPtr const &newN2 = mapGet(this->nodes, newN2Id);
            NodeTPtr const &n2 = edge->getN2();
            NodeId const &n1Id = edge->getN1()->getId(), &n2Id = n2->getId();

            if (n1Id == newN1Id && n2Id == newN2Id) {
                return;
            }
            if (n2Id != newN2Id) {
                // change the incoming node of the edge
                mapDelete(this->incomingEdges[n2Id], edge);
                this->incomingEdges[newN2Id][edge] = true;
            }
            if (n1Id != newN1Id) {
                // change the outgoing node of the edge
                mapDelete(this->outgoingEdges[n1Id], edge);
                this->outgoingEdges[newN1Id][edge] = true;
            }

            assert(this->neighbors[n1Id][n2] > 0);
            this->neighbors[n1Id][n2]--;
            if (this->neighbors[n1Id][n2] == 0) {
                mapDelete(this->neighbors[n1Id], n2);
            }
            this->neighbors[newN1Id][newN2]++;

            mapDelete(this->edgesFromNodeIds[{n1Id, n2Id}], edge);
            this->edgesFromNodeIds[{newN1Id, newN2Id}][edge] = true;

            edge->update(newEdgeId, newN1, newN2);
            if (edgeId != newEdgeId) {
                // first add edge to new map and then delete it from the original container
                mapAdd<EdgeId, EdgeTPtr const>(this->edges, newEdgeId, edge);
                mapDelete(this->edges, edgeId);
            }
        }

    protected:
        void addNodeImpl(NodeTPtr const &node) {
            if (node == nullptr) {
                throw std::runtime_error("Node can not be a nullptr!");
            }
            if (mapContains(this->nodes, node->getId())) {
                throw std::runtime_error("Node is already present in the graph!");
            }
            mapAdd<NodeId, NodeTPtr const>(this->nodes, node->getId(), node);
            this->incomingEdges[node->getId()];  // create the empty-map value
            this->outgoingEdges[node->getId()];  // create the empty-map value
        }

        void addEdgeImpl(EdgeTPtr const &edge) {
            if (edge == nullptr) {
                throw std::runtime_error("Edge can not be a nullptr!");
            }
            if (edge->getN1() == nullptr || edge->getN2() == nullptr) {
                throw std::runtime_error("An edge's node can not be a nullptr!");
            }
            if (!this->withSelfEdges && edge->getN1()->getId() == edge->getN2()->getId()) {
                throw std::runtime_error("WithSelfEdges is disabled! Can not add edge...");
            }
            if (!this->withMultipleEdges && this->neighbors[edge->getN1()->getId()][edge->getN2()] != 0) {
                throw std::runtime_error("WithMultipleEdges is disabled! Can not add edge...");
            }
            if (!mapContains(this->nodes, edge->getN1()->getId())) {
                throw std::runtime_error("Edge's n1 is not present in the graph's nodes. Add it first!");
            }
            if (!mapContains(this->nodes, edge->getN2()->getId())) {
                throw std::runtime_error("Edge's n2 is not present in the graph's nodes. Add it first!");
            }
            if (mapContains(this->edges, edge->getId())) {  // even if edge have same nodes, assign different id!
                throw std::runtime_error("Edge is already present in the graph!");
            }
            mapAdd<EdgeId, EdgeTPtr const>(this->edges, edge->getId(), edge);
            this->edgesFromNodeIds[{edge->getN1()->getId(), edge->getN2()->getId()}][edge] = true;
            this->neighbors[edge->getN1()->getId()][edge->getN2()]++;
            this->incomingEdges[edge->getN2()->getId()][edge] = true;
            this->outgoingEdges[edge->getN1()->getId()][edge] = true;
        }

        void removeNode(NodeTPtr const &node) {
            // delete node from edgesFromNodeIds
            auto nodeId = node->getId();
            std::vector<EdgeTPtr const *> toRemoveEdges;
            std::vector<NodeId> toRemoveEdgeNodeIds;
            for (auto const &edgeData: mapGet(this->incomingEdges, nodeId)) {
                toRemoveEdges.push_back(&edgeData.first);
                toRemoveEdgeNodeIds.push_back(edgeData.first->getN1()->getId());
            }
            for (size_t i = 0; i < toRemoveEdges.size(); i++) {
                auto &n1Id = toRemoveEdgeNodeIds[i];
                mapDelete(this->edgesFromNodeIds, std::make_pair(n1Id, nodeId));
                this->removeEdge(*toRemoveEdges[i]);
                // Delete all the neighbors where node is an incoming node
                mapDelete(this->neighbors[n1Id], node);
            }
            toRemoveEdges.clear();
            toRemoveEdgeNodeIds.clear();
            for (auto const &edgeData: mapGet(this->outgoingEdges, nodeId)) {
                toRemoveEdges.push_back(&edgeData.first);
                toRemoveEdgeNodeIds.push_back(edgeData.first->getN1()->getId());
            }
            for (size_t i = 0; i < toRemoveEdges.size(); i++) {
                mapDelete(this->edgesFromNodeIds, std::make_pair(nodeId, toRemoveEdgeNodeIds[i]));
                this->removeEdge(*toRemoveEdges[i]);
            }
            mapDelete(this->incomingEdges, nodeId);
            mapDelete(this->outgoingEdges, nodeId);

            mapDelete(this->neighbors, nodeId);
            this->deleteNode(nodeId);
        }

        void removeEdge(EdgeTPtr const &edge) {
            mapDelete(this->incomingEdges[edge->getN2()->getId()], edge);
            mapDelete(this->outgoingEdges[edge->getN1()->getId()], edge);

            auto edgeId = edge->getId();
            assert(this->neighbors[edge->getN1()->getId()][edge->getN2()] > 0);
            this->neighbors[edge->getN1()->getId()][edge->getN2()]--;
            if (this->neighbors[edge->getN1()->getId()][edge->getN2()] == 0) {
                mapDelete(this->neighbors[edge->getN1()->getId()], edge->getN2());
            }

            this->deleteEdge(edgeId);
        }

        void deleteNode(NodeId nodeId) {
            // don't make nodeId a reference; once node is deleted the reference points to bad memory
            mapDelete(this->nodes, nodeId);
        }

        void deleteEdge(EdgeId edgeId) {
            // don't make edgeId a reference; once edge is deleted the reference points to bad memory
            mapDelete(this->edges, edgeId);
        }

        bool withMultipleEdges, withSelfEdges;
        std::map<NodeId, NodeTPtr const> nodes;
        std::map<EdgeId, EdgeTPtr const> edges;
        std::map<NodeId, std::map<EdgeTPtr, bool>> incomingEdges, outgoingEdges;
        std::map<std::pair<NodeId, NodeId>, std::map<EdgeTPtr, bool>> edgesFromNodeIds;
        std::map<NodeId, std::map<NodeTPtr, int>> neighbors;  // outgoing neighbors; int for counting multiple edges
    };
}

#endif //ANDREIUTILS_GRAPH_HPP
