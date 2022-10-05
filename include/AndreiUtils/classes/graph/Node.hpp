//
// Created by Andrei Costinescu on 23.06.22.
//

#ifndef ANDREIUTILS_NODE_HPP
#define ANDREIUTILS_NODE_HPP

#include <AndreiUtils/classes/graph/NodeData.h>
#include <type_traits>
#include <utility>

namespace AndreiUtils {
    template<typename NodeId=int>
    class Node {
    public:
        Node() : id(), data(nullptr) {}

        explicit Node(NodeId const &id) : id(id), data(nullptr), ownsData(false) {}

        Node(NodeId const &id, NodeData *data, bool passOwnership = false) :
                id(id), data(data), ownsData(passOwnership) {}

        // this only accepts r-values as the data parameter
        template<class T>
        Node(NodeId const &id, T &&data) : id(id), data(nullptr), ownsData(true) {
            static_assert(std::is_base_of<NodeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::NodeData");
            this->data = new T(std::move(data));
        }

        // needed for the above to only accept r-values
        template<class T>
        Node(NodeId const &id, T &data) = delete;

        Node(Node const &other) : id(other.id), data(other.data), ownsData(false) {}

        Node(Node &&other) noexcept: id(other.id), data(other.data), ownsData(other.ownsData) {
            other.ownsData = false;
            other.reset();
        }

        Node &operator=(Node const &other) {
            if (&other != this) {
                this->discardData();
                this->id = other.id;
                this->data = other.data;
                this->ownsData = false;
            }
            return *this;
        }

        Node &operator=(Node &&other) noexcept {
            if (&other != this) {
                this->discardData();
                this->id = other.id;
                this->data = other.data;
                this->ownsData = other.ownsData;
                other.ownsData = false;
                other.reset();
            }
            return *this;
        }

        virtual ~Node() {
            this->reset();
        }

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

        template<class T>
        T *getData() const {
            static_assert(std::is_base_of<NodeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::NodeData");
            return dynamic_cast<T *>(this->data);
        }

        void setData(NodeData *_data, bool passOwnership = false) {
            this->discardData();
            this->data = _data;
            this->ownsData = passOwnership;
        }

        // this only accepts r-values as the _data parameter
        template<class T>
        void setData(T &&_data) {
            static_assert(std::is_base_of<NodeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::NodeData");
            this->discardData();
            this->data = new T(std::move(_data));
            this->ownsData = true;
        }

        // needed for the above to only accept r-values
        template<class T>
        void setData(T &_data) = delete;

    protected:
        void reset() {
            this->discardData();
            this->ownsData = false;
            this->data = nullptr;
        }

        void discardData() {
            if (this->ownsData) {
                delete this->data;
                this->data = nullptr;
                this->ownsData = false;
            }
        }

        NodeId id;
        NodeData *data;
        bool ownsData;
    };
}

#endif //ANDREIUTILS_NODE_HPP
