//
// Created by Andrei Costinescu on 23.06.22.
//

#ifndef ANDREIUTILS_EDGE_HPP
#define ANDREIUTILS_EDGE_HPP

#include <AndreiUtils/classes/graph/EdgeData.h>
#include <AndreiUtils/classes/graph/Node.hpp>
#include <functional>
#include <string>

namespace AndreiUtils {
    template<typename EdgeId=std::string, typename NodeId=int>
    class Edge {
        using NodeT = Node<NodeId>;
    public:
        Edge() : id(), n1(nullptr), n2(nullptr), data(nullptr) {}

        Edge(EdgeId id, NodeT *n1, NodeT *n2) : id(id), n1(n1), n2(n2), data(nullptr) {}

        Edge(EdgeId id, NodeT &n1, NodeT &n2) : id(id), n1(&n1), n2(&n2), data(nullptr) {}

        Edge(EdgeId id, NodeT *n1, NodeT *n2, EdgeData *data) : id(id), n1(n1), n2(n2), data(data) {}

        Edge(EdgeId id, NodeT &n1, NodeT &n2, EdgeData *data) : id(id), n1(&n1), n2(&n2), data(data) {}

        Edge(EdgeId id, NodeT *n1, NodeT *n2, EdgeData &data) : id(id), n1(n1), n2(n2), data(&data) {}

        Edge(EdgeId id, NodeT &n1, NodeT &n2, EdgeData &data) : id(id), n1(&n1), n2(&n2), data(&data) {}

        Edge(NodeT &n1, NodeT &n2, std::function<EdgeId(NodeT const &n1, NodeT const &n2)> const &createIdFromNodes) :
                id(createIdFromNodes(n1, n2)), n1(&n1), n2(&n2), data(nullptr) {}

        Edge(NodeT &n1, NodeT &n2, std::function<EdgeId(NodeT const &n1, NodeT const &n2)> const &createIdFromNodes,
             EdgeData *data) : id(createIdFromNodes(n1, n2)), n1(&n1), n2(&n2), data(data) {}

        Edge(NodeT &n1, NodeT &n2, std::function<EdgeId(NodeT const &n1, NodeT const &n2)> const &createIdFromNodes,
             EdgeData &data) : id(createIdFromNodes(n1, n2)), n1(&n1), n2(&n2), data(&data) {}

        virtual ~Edge() = default;

        inline EdgeId &getId() {
            return this->id;
        }

        inline EdgeId const &getId() const {
            return this->id;
        }

        inline Node<NodeId> *&getN1() {
            return this->n1;
        }

        inline Node<NodeId> *const &getN1() const {
            return this->n1;
        }

        inline Node<NodeId> *&getN2() {
            return this->n2;
        }

        inline Node<NodeId> *const &getN2() const {
            return this->n2;
        }

        inline EdgeData *&getData() {
            return this->data;
        }

        inline EdgeData *const &getData() const {
            return this->data;
        }

        void update(EdgeId const &_id, NodeT *const &_n1, NodeT *const &_n2) {
            this->id = _id;
            this->n1 = _n1;
            this->n2 = _n2;
        }

        void update(EdgeId const &_id, NodeT *const &_n1, NodeT *const &_n2, EdgeData &_data) {
            this->update(_id, _n1, _n2);
            this->data = &_data;
        }

    protected:
        EdgeId id;
        NodeT *n1, *n2;
        EdgeData *data;
    };
}

#endif //ANDREIUTILS_EDGE_HPP
