//
// Created by Andrei Costinescu on 23.06.22.
//

#ifndef ANDREIUTILS_NODE_HPP
#define ANDREIUTILS_NODE_HPP

#include <AndreiUtils/classes/graph/NodeData.h>
#include <memory>
#include <type_traits>
#include <utility>

namespace AndreiUtils {
    template<typename NodeId=int>
    class Node {
    public:
        Node() : id(), data(nullptr) {}

        explicit Node(NodeId id) : id(std::move(id)), data(nullptr) {}

        Node(NodeId id, std::shared_ptr<NodeData> data) : id(std::move(id)), data(std::move(data)) {}

        // this only accepts r-values as the data parameter
        template<class T>
        Node(NodeId id, T &&data) : id(std::move(id)), data(nullptr) {
            static_assert(std::is_base_of<NodeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::NodeData");
            this->data = std::make_shared<T>(std::move(data));
        }

        // needed for the above to only accept r-values
        template<class T>
        Node(NodeId const &id, T &data) = delete;

        Node(Node const &other) : id(other.id), data(other.data) {}

        Node(Node &&other) noexcept: id(std::move(other.id)), data(std::move(other.data)) {}

        Node &operator=(Node const &other) {
            if (&other != this) {
                this->id = other.id;
                this->data = other.data;
            }
            return *this;
        }

        Node &operator=(Node &&other) noexcept {
            if (&other != this) {
                this->id = std::move(other.id);
                this->data = std::move(other.data);
            }
            return *this;
        }

        virtual ~Node() {
            this->data.reset();
        }

        inline NodeId &getId() {
            return this->id;
        }

        inline NodeId const &getId() const {
            return this->id;
        }

        inline std::shared_ptr<NodeData> &getData() {
            return this->data;
        }

        inline std::shared_ptr<NodeData> const &getData() const {
            return this->data;
        }

        template<class T>
        T *getDataPtr() const {
            static_assert(std::is_base_of<NodeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::NodeData");
            return dynamic_cast<T *>(this->data.get());
        }

        template<class T>
        std::shared_ptr<T> getData() const {
            static_assert(std::is_base_of<NodeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::NodeData");
            return std::dynamic_pointer_cast<T>(this->data);
        }

        void setData(std::shared_ptr<NodeData> _data) {
            this->data = std::move(_data);  // this resets the pointer internally
        }

        // this only accepts r-values as the _data parameter
        template<class T>
        void setData(T &&_data) {
            static_assert(std::is_base_of<NodeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::NodeData");
            this->data = std::make_shared<T>(std::move(_data));
        }

        // needed for the above to only accept r-values
        template<class T>
        void setData(T &_data) = delete;

    protected:

        NodeId id;
        std::shared_ptr<NodeData> data;
    };
}

#endif //ANDREIUTILS_NODE_HPP
