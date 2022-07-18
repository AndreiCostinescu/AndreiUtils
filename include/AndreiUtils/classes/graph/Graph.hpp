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
        using EdgeT = Edge<EdgeId, NodeId>;
    public:
        explicit Graph(bool withMultipleEdges = false, bool withSelfEdges = false) :
                withMultipleEdges(withMultipleEdges), withSelfEdges(withSelfEdges) {}

        Graph(Graph const &other) :
                nodes(other.nodes), edges(other.edges), edgesFromNodeIds(other.edgesFromNodeIds),
                neighbors(other.neighbors), withMultipleEdges(other.withMultipleEdges),
                withSelfEdges(other.withSelfEdges), incomingEdges(other.incomingEdges),
                outgoingEdges(other.outgoingEdges) {
            for (auto const &allocatedData: other.allocatedNodes) {
                this->allocatedNodes[allocatedData.first] = false;
            }
            for (auto const &allocatedData: other.allocatedEdges) {
                this->allocatedEdges[allocatedData.first] = false;
            }
        }

        Graph(Graph &&other) noexcept:
                nodes(std::move(other.nodes)), edges(std::move(other.edges)),
                edgesFromNodeIds(std::move(other.edgesFromNodeIds)), neighbors(std::move(other.neighbors)),
                allocatedNodes(std::move(other.allocatedNodes)), allocatedEdges(std::move(other.allocatedEdges)),
                withMultipleEdges(std::move(other.withMultipleEdges)), incomingEdges(std::move(other.incomingEdges)),
                outgoingEdges(std::move(other.outgoingEdges)), withSelfEdges(std::move(other.withSelfEdges)) {}

        virtual ~Graph() {
            for (auto const &allocatedData: this->allocatedNodes) {
                if (allocatedData.second) {
                    delete mapGet(this->nodes, allocatedData.first);
                }
            }
            for (auto const &allocatedData: this->allocatedEdges) {
                if (allocatedData.second) {
                    delete mapGet(this->edges, allocatedData.first);
                }
            }
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
            for (auto const &allocatedData: other.allocatedNodes) {
                this->allocatedNodes[allocatedData.first] = false;
            }
            for (auto const &allocatedData: other.allocatedEdges) {
                this->allocatedEdges[allocatedData.first] = false;
            }
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
            this->allocatedNodes = std::move(other.allocatedNodes);
            this->allocatedEdges = std::move(other.allocatedEdges);
            other.clear();
            return *this;
        }

        void clear() {
            this->withMultipleEdges = false;
            this->withSelfEdges = false;
            this->nodes.clear();
            this->allocatedNodes.clear();
            this->edges.clear();
            this->allocatedEdges.clear();
            this->incomingEdges.clear();
            this->outgoingEdges.clear();
            this->edgesFromNodeIds.clear();
            this->neighbors.clear();
        }

        void addNode(NodeId const &n) {
            this->addNode(new NodeT(n), true);
        }

        void addNode(NodeT n) {
            this->addNode(new NodeT(n), true);
        }

        void addNode(NodeT &n) {
            this->addNode(&n, false);
        }

        void removeNode(NodeId const &nodeId) {
            NodeT *const &node = mapGet(this->nodes, nodeId);
            this->removeNode(node);
        }

        void removeNode(NodeT const &node) {
            this->removeNode(&node);
        }

        void addEdge(NodeId const &n1, NodeId const &n2,
                     std::function<EdgeId(NodeId const &n1, NodeId const &n2)> const &createEdgeId) {
            this->addEdge(new EdgeT{*mapGet(this->nodes, n1), *mapGet(this->nodes, n2),
                                    [&](NodeT const &_n1, NodeT const &_n2) {
                                        return createEdgeId(_n1.getId(), _n2.getId());
                                    }}, true);
        }

        void addEdge(EdgeT e) {
            this->addEdge(new EdgeT(e), true);
        }

        void addEdge(EdgeT &e) {
            this->addEdge(&e, false);
        }

        void addUndirectedEdge(NodeId const &n1, NodeId const &n2,
                               std::function<EdgeId(NodeId const &n1, NodeId const &n2)> const &createEdgeId) {
            auto convert = [&](NodeT const &_n1, NodeT const &_n2) { return createEdgeId(_n1.getId(), _n2.getId()); };
            this->addEdge(new EdgeT{*mapGet(this->nodes, n1), *mapGet(this->nodes, n2), convert}, true);
            this->addEdge(new EdgeT{*mapGet(this->nodes, n2), *mapGet(this->nodes, n1), convert}, true);
        }

        void removeEdge(EdgeId const &edgeId) {
            EdgeT *const &edge = mapGet(this->edges, edgeId);
            this->removeEdge(edge);
        }

        void removeEdge(EdgeT const &edge) {
            this->removeEdge(&edge);
        }

        void removeEdges(NodeId const &n1, NodeId const &n2) {
            for (auto const &edge: mapGet(this->edgesFromNodeIds, std::make_pair(n1, n2))) {
                this->removeEdge(edge.first);
            }
        }

        inline size_t getNrNodes() const {
            return this->nodes.size();
        }

        inline size_t getNrEdges() const {
            return this->edges.size();
        }

        std::map<NodeId, NodeT *const> &getNodes() {
            return this->nodes;
        }

        std::map<NodeId, NodeT *const> const &getNodes() const {
            return this->nodes;
        }

        std::map<EdgeId, EdgeT *const> &getEdges() {
            return this->edges;
        }

        std::map<EdgeId, EdgeT *const> const &getEdges() const {
            return this->edges;
        }

        bool hasNode(NodeId const &nodeId) const {
            return mapContains(this->nodes, nodeId);
        }

        bool hasEdge(EdgeId const &edgeId) const {
            return mapContains(this->edges, edgeId);
        }

        NodeT const *getNode(NodeId const &nodeId) const {
            return mapGet(this->nodes, nodeId);
        }

        EdgeT const *getEdge(EdgeId const &edgeId) const {
            return mapGet(this->edges, edgeId);
        }

        std::map<EdgeT *, bool> const &getEdges(std::pair<NodeId, NodeId> const &edgeId) const {
            return mapGet(this->edgesFromNodeIds, edgeId);
        }

        std::map<NodeId, std::map<NodeT *, int>> const &getNeighbors() const {
            return this->neighbors;
        }

        std::map<NodeT *, int> const *getNeighbors(NodeT const *const n) const {
            return this->getNeighbors(n->getId());
        }

        std::map<NodeT *, int> const *getNeighbors(NodeId const &nId) const {
            if (!mapContains(this->neighbors, nId)) {
                return nullptr;
            }
            return &mapGet(this->neighbors, nId);
        }

        std::map<NodeId, std::map<EdgeT *, bool>> &getIncomingEdgesForEachNode() {
            return this->incomingEdges;
        }

        std::map<NodeId, std::map<EdgeT *, bool>> const &getIncomingEdgesForEachNode() const {
            return this->incomingEdges;
        }

        std::map<NodeId, std::map<EdgeT *, bool>> &getOutgoingEdgesForEachNode() {
            return this->outgoingEdges;
        }

        std::map<NodeId, std::map<EdgeT *, bool>> const &getOutgoingEdgesForEachNode() const {
            return this->outgoingEdges;
        }

        void changeEdge(EdgeId const &edgeId, EdgeId const &newEdgeId, NodeId const &newN1Id, NodeId const &newN2Id) {
            auto &edge = mapGet(this->edges, edgeId);
            NodeT *const &newN1 = mapGet(this->nodes, newN1Id);
            NodeT *const &newN2 = mapGet(this->nodes, newN2Id);
            NodeT *const &n2 = edge->getN2();
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
                mapDelete(this->edges, edgeId);
                mapAdd<EdgeId, EdgeT *const>(this->edges, newEdgeId, edge);
                this->allocatedEdges[newEdgeId] = this->allocatedEdges[edgeId];
                mapDelete(this->allocatedEdges, edgeId);
            }
        }

    protected:
        void addNode(NodeT *node, bool allocatedData) {
            if (node == nullptr) {
                throw std::runtime_error("Node can not be a nullptr!");
            }
            if (mapContains(this->nodes, node->getId())) {
                throw std::runtime_error("Node is already present in the graph!");
            }
            mapAdd<NodeId, NodeT *const>(this->nodes, node->getId(), node);
            this->allocatedNodes[node->getId()] = allocatedData;
            this->incomingEdges[node->getId()];  // create the empty-map value
            this->outgoingEdges[node->getId()];  // create the empty-map value
        }

        void addEdge(EdgeT *edge, bool allocatedData) {
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
            if (mapContains(this->edges, edge->getId())) {
                throw std::runtime_error("Edge is already present in the graph!");
            }
            mapAdd<EdgeId, EdgeT *const>(this->edges, edge->getId(), edge);
            this->allocatedEdges[edge->getId()] = allocatedData;
            this->edgesFromNodeIds[{edge->getN1()->getId(), edge->getN2()->getId()}][edge] = true;
            this->neighbors[edge->getN1()->getId()][edge->getN2()]++;
            this->incomingEdges[edge->getN2()->getId()][edge] = true;
            this->outgoingEdges[edge->getN1()->getId()][edge] = true;
        }

        void removeNode(NodeT const *node) {
            // delete node from edgesFromNodeIds
            auto nodeId = node->getId();
            std::vector<EdgeT *> toRemoveEdges;
            std::vector<NodeId> toRemoveEdgeNodeIds;
            for (auto const &edgeData: mapGet(this->incomingEdges, nodeId)) {
                toRemoveEdges.push_back(edgeData.first);
                toRemoveEdgeNodeIds.push_back(edgeData.first->getN1()->getId());
            }
            for (size_t i = 0; i < toRemoveEdges.size(); i++) {
                auto &n1Id = toRemoveEdgeNodeIds[i];
                mapDelete(this->edgesFromNodeIds, std::make_pair(n1Id, nodeId));
                this->removeEdge(toRemoveEdges[i]);
                // Delete all the neighbors where node is an incoming node
                mapDelete(this->neighbors[n1Id], node);
            }
            toRemoveEdges.clear();
            toRemoveEdgeNodeIds.clear();
            for (auto const &edgeData: mapGet(this->outgoingEdges, nodeId)) {
                toRemoveEdges.push_back(edgeData.first);
                toRemoveEdgeNodeIds.push_back(edgeData.first->getN1()->getId());
            }
            for (size_t i = 0; i < toRemoveEdges.size(); i++) {
                mapDelete(this->edgesFromNodeIds, std::make_pair(nodeId, toRemoveEdgeNodeIds[i]));
                this->removeEdge(toRemoveEdges[i]);
            }
            mapDelete(this->incomingEdges, nodeId);
            mapDelete(this->outgoingEdges, nodeId);

            mapDelete(this->neighbors, nodeId);
            this->deleteNode(node, nodeId);
        }

        void removeEdge(EdgeT const *edge) {
            mapDelete(this->incomingEdges[edge->getN2()->getId()], edge);
            mapDelete(this->outgoingEdges[edge->getN1()->getId()], edge);

            auto edgeId = edge->getId();
            assert(this->neighbors[edge->getN1()->getId()][edge->getN2()] > 0);
            this->neighbors[edge->getN1()->getId()][edge->getN2()]--;
            if (this->neighbors[edge->getN1()->getId()][edge->getN2()] == 0) {
                mapDelete(this->neighbors[edge->getN1()->getId()], edge->getN2());
            }

            this->deleteEdge(edge, edgeId);
        }

        void deleteNode(NodeT const *node, NodeId nodeId) {
            // don't make nodeId a reference; once node is deleted the reference points to bad memory
            if (mapGet(this->allocatedNodes, nodeId)) {
                delete node;
            }
            mapDelete(this->nodes, nodeId);
            mapDelete(this->allocatedNodes, nodeId);
        }

        void deleteEdge(EdgeT const *edge, EdgeId edgeId) {
            // don't make edgeId a reference; once edge is deleted the reference points to bad memory
            if (mapGet(this->allocatedEdges, edgeId)) {
                delete edge;
            }
            mapDelete(this->edges, edgeId);
            mapDelete(this->allocatedEdges, edgeId);
        }

        bool withMultipleEdges, withSelfEdges;
        std::map<NodeId, NodeT *const> nodes;
        std::map<NodeId, bool> allocatedNodes;
        std::map<EdgeId, EdgeT *const> edges;
        std::map<EdgeId, bool> allocatedEdges;
        std::map<NodeId, std::map<EdgeT *, bool>> incomingEdges, outgoingEdges;
        std::map<std::pair<NodeId, NodeId>, std::map<EdgeT *, bool>> edgesFromNodeIds;
        std::map<NodeId, std::map<NodeT *, int>> neighbors;  // outgoing neighbors
    };
}

#endif //ANDREIUTILS_GRAPH_HPP
