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
        using NodeTPtr = std::shared_ptr<Node<NodeId>>;
        using EdgeDataPtr = std::shared_ptr<EdgeData>;
        using EdgeIdFunction = std::function<EdgeId(NodeT const &, NodeT const &)>;
    public:
        Edge() : id(), n1(nullptr), n2(nullptr), data(nullptr) {}

        Edge(EdgeId id, NodeTPtr n1, NodeTPtr n2) :
                id(std::move(id)), n1(std::move(n1)), n2(std::move(n2)), data(nullptr) {}

        Edge(EdgeId id, NodeTPtr n1, NodeTPtr n2, EdgeDataPtr data) :
                id(std::move(id)), n1(std::move(n1)), n2(std::move(n2)), data(std::move(data)) {}

        // this only accepts r-values as data parameter
        template<class T>
        Edge(EdgeId id, NodeTPtr n1, NodeTPtr n2, T &&data) :
                id(std::move(id)), n1(std::move(n1)), n2(std::move(n2)), data(nullptr) {
            static_assert(std::is_base_of<EdgeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::EdgeData");
            this->data = std::make_shared<T>(std::move(data));
        }

        // needed for the above to only accept r-values
        template<class T>
        Edge(EdgeId id, NodeTPtr n1, NodeTPtr n2, T &data) = delete;

        Edge(NodeTPtr const &n1, NodeTPtr const &n2, EdgeIdFunction const &createIdFromNodes) :
                id(createIdFromNodes(*n1, *n2)), n1(n1), n2(n2), data(nullptr) {}

        Edge(NodeTPtr const &n1, NodeTPtr const &n2, EdgeIdFunction const &createIdFromNodes, EdgeDataPtr data) :
                id(createIdFromNodes(*n1, *n2)), n1(n1), n2(n2), data(std::move(data)) {}

        // the constructor only accepts r-values as data parameter
        template<class T>
        Edge(NodeTPtr const &n1, NodeTPtr const &n2, EdgeIdFunction const &createIdFromNodes, T &&data) :
                id(createIdFromNodes(*n1, *n2)), n1(n1), n2(n2), data(nullptr) {
            static_assert(std::is_base_of<EdgeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::EdgeData");
            this->data = std::make_shared<T>(std::move(data));
        }

        // needed for the above to only accept r-values
        template<class T>
        Edge(NodeTPtr const &n1, NodeTPtr const &n2, EdgeIdFunction const &createIdFromNodes, T &data) = delete;

        Edge(Edge const &other) : id(other.id), n1(other.n1), n2(other.n2), data(other.data) {}

        Edge(Edge &&other) noexcept: id(std::move(other.id)), n1(std::move(other.n1)), n2(std::move(other.n2)),
                                     data(std::move(other.data)) {}

        Edge &operator=(Edge const &other) {
            if (&other != this) {
                this->id = other.id;
                this->n1 = other.n1;
                this->n2 = other.n2;
                this->data = other.data;
            }
            return *this;
        }

        Edge &operator=(Edge &&other) noexcept {
            if (&other != this) {
                this->id = std::move(other.id);
                this->n1 = std::move(other.n1);
                this->n2 = std::move(other.n2);
                this->data = std::move(other.data);
            }
            return *this;
        }

        virtual ~Edge() {
            this->data.reset();
        }

        inline EdgeId &getId() {
            return this->id;
        }

        inline EdgeId const &getId() const {
            return this->id;
        }

        inline NodeTPtr &getN1() {
            return this->n1;
        }

        inline NodeTPtr const &getN1() const {
            return this->n1;
        }

        inline NodeTPtr &getN2() {
            return this->n2;
        }

        inline NodeTPtr const &getN2() const {
            return this->n2;
        }

        inline EdgeDataPtr &getData() {
            return this->data;
        }

        inline EdgeDataPtr const &getData() const {
            return this->data;
        }

        template<class T>
        T *getDataPtr() const {
            static_assert(std::is_base_of<EdgeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::EdgeData");
            return dynamic_cast<T *>(this->data.get());
        }

        template<class T>
        std::shared_ptr<T> getData() const {
            static_assert(std::is_base_of<EdgeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::EdgeData");
            return std::dynamic_pointer_cast<T>(this->data);
        }

        void setData(EdgeDataPtr _data) {
            this->data = std::move(_data);
        }

        // the function only accepts r-values as _data parameter
        template<class T>
        void setData(T &&_data) {
            static_assert(std::is_base_of<EdgeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::EdgeData");
            this->data = std::make_shared<T>(std::move(_data));
        }

        // needed for the above to only accept r-values
        template<class T>
        void setData(T &_data) = delete;

        void update(EdgeId const &_id, NodeTPtr const &_n1, NodeTPtr const &_n2) {
            this->id = _id;
            this->n1 = _n1;
            this->n2 = _n2;
        }

        void update(EdgeId const &_id, NodeTPtr const &_n1, NodeTPtr const &_n2, EdgeDataPtr _data) {
            this->update(_id, _n1, _n2);
            this->setData(std::move(_data));
        }

    protected:
        EdgeId id;
        std::shared_ptr<NodeT> n1, n2;
        std::shared_ptr<EdgeData> data;
    };
}

#endif //ANDREIUTILS_EDGE_HPP
