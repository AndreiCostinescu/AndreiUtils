//
// Created by Andrei Costinescu on 23.06.22.
//

#ifndef ANDREIUTILS_NODE_HPP
#define ANDREIUTILS_NODE_HPP

#include <AndreiUtils/classes/graph/NodeData.h>

namespace AndreiUtils {
    template<typename NodeId=int>
    class Node {
    public:
        Node() : id(), data(nullptr) {}

        explicit Node(NodeId const &id) : id(id), data(nullptr) {}

        Node(NodeId const &id, NodeData *data) : id(id), data(data) {}

        Node(NodeId const &id, NodeData &data) : id(id), data(&data) {}

        virtual ~Node() = default;

        inline NodeId &getId() {
            return this->id;
        }

        inline NodeId const &getId() const {
            return this->id;
        }

        inline NodeData *&getData() {
            return this->data;
        }

        inline NodeData *const &getData() const {
            return this->data;
        }

    protected:
        NodeId id;
        NodeData *data;
    };
}

#endif //ANDREIUTILS_NODE_HPP
