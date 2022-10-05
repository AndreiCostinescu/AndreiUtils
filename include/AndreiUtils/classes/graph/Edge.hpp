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
        using EdgeIdFunction = std::function<EdgeId(NodeT const &, NodeT const &)>;
    public:
        Edge() : id(), n1(nullptr), n2(nullptr), data(nullptr), ownsData(false) {}

        Edge(EdgeId id, NodeT *n1, NodeT *n2) : id(id), n1(n1), n2(n2), data(nullptr), ownsData(false) {}

        Edge(EdgeId id, NodeT &n1, NodeT &n2) : id(id), n1(&n1), n2(&n2), data(nullptr), ownsData(false) {}

        Edge(EdgeId id, NodeT *n1, NodeT *n2, EdgeData *data, bool passOwnership = false) :
                id(id), n1(n1), n2(n2), data(data), ownsData(passOwnership) {}

        // this only accepts r-values as data parameter
        template<class T>
        Edge(EdgeId id, NodeT *n1, NodeT *n2, T &&data) :
                id(id), n1(n1), n2(n2), data(nullptr), ownsData(true) {
            static_assert(std::is_base_of<EdgeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::EdgeData");
            this->data = new T(std::move(data));
        }

        // needed for the above to only accept r-values
        template<class T>
        Edge(EdgeId id, NodeT *n1, NodeT *n2, T &data) = delete;

        Edge(EdgeId id, NodeT &n1, NodeT &n2, EdgeData *data, bool passOwnership = false) :
                id(id), n1(&n1), n2(&n2), data(data), ownsData(passOwnership) {}

        // this only accepts r-values as data parameter
        template<class T>
        Edge(EdgeId id, NodeT &n1, NodeT &n2, T &&data) :
                id(id), n1(&n1), n2(&n2), data(nullptr), ownsData(true) {
            static_assert(std::is_base_of<EdgeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::EdgeData");
            this->data = new T(std::move(data));
        }

        // needed for the above to only accept r-values
        template<class T>
        Edge(EdgeId id, NodeT &n1, NodeT &n2, T &data) = delete;

        Edge(NodeT &n1, NodeT &n2, EdgeIdFunction const &createIdFromNodes) :
                id(createIdFromNodes(n1, n2)), n1(&n1), n2(&n2), data(nullptr), ownsData(false) {}

        Edge(NodeT &n1, NodeT &n2, EdgeIdFunction const &createIdFromNodes, EdgeData *data, bool passOwnership = false)
                : id(createIdFromNodes(n1, n2)), n1(&n1), n2(&n2), data(data), ownsData(passOwnership) {}

        // the constructor only accepts r-values as data parameter
        template<class T>
        Edge(NodeT &n1, NodeT &n2, EdgeIdFunction const &createIdFromNodes, T &&data) :
                id(createIdFromNodes(n1, n2)), n1(&n1), n2(&n2), data(nullptr), ownsData(true) {
            static_assert(std::is_base_of<EdgeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::EdgeData");
            this->data = new T(std::move(data));
        }

        // needed for the above to only accept r-values
        template<class T>
        Edge(NodeT &n1, NodeT &n2, EdgeIdFunction const &createIdFromNodes, T &data) = delete;

        Edge(Edge const &other) : id(other.id), n1(other.n1), n2(other.n2), data(other.data), ownsData(false) {}

        Edge(Edge &&other) noexcept: id(other.id), n1(other.n1), n2(other.n2), data(other.data),
                                     ownsData(other.ownsData) {
            other.ownsData = false;
            other.reset();
        }

        Edge &operator=(Edge const &other) {
            if (&other != this) {
                this->discardData();
                this->id = other.id;
                this->n1 = other.n1;
                this->n2 = other.n2;
                this->data = other.data;
                this->ownsData = false;
            }
            return *this;
        }

        Edge &operator=(Edge &&other) noexcept {
            if (&other != this) {
                this->discardData();
                this->id = other.id;
                this->n1 = other.n1;
                this->n2 = other.n2;
                this->data = other.data;
                this->ownsData = other.ownsData;
                other.ownsData = false;
                other.reset();
            }
            return *this;
        }

        virtual ~Edge() {
            this->reset();
        }

        inline EdgeId &getId() {
            return this->id;
        }

        inline EdgeId const &getId() const {
            return this->id;
        }

        inline NodeT *&getN1() {
            return this->n1;
        }

        inline NodeT *const &getN1() const {
            return this->n1;
        }

        inline NodeT *&getN2() {
            return this->n2;
        }

        inline NodeT *const &getN2() const {
            return this->n2;
        }

        inline EdgeData *&getData() {
            return this->data;
        }

        inline EdgeData *const &getData() const {
            return this->data;
        }

        void setData(EdgeData *_data, bool passOwnership = false) {
            this->discardData();
            this->data = _data;
            this->ownsData = passOwnership;
        }

        // the function only accepts r-values as _data parameter
        template<class T>
        void setData(T &&_data) {
            static_assert(std::is_base_of<EdgeData, T>::value,
                          "The template parameter T is not a derived class of AndreiUtils::EdgeData");
            this->discardData();
            this->data = new T(std::move(_data));
            this->ownsData = true;
        }

        // needed for the above to only accept r-values
        template<class T>
        void setData(T &_data) = delete;

        void update(EdgeId const &_id, NodeT *const &_n1, NodeT *const &_n2) {
            this->id = _id;
            this->n1 = _n1;
            this->n2 = _n2;
        }

        void update(EdgeId const &_id, NodeT *const &_n1, NodeT *const &_n2, EdgeData *_data,
                    bool passOwnership = false) {
            this->update(_id, _n1, _n2);
            this->setData(_data, passOwnership);
        }

    protected:
        void reset() {
            this->discardData();
            this->ownsData = false;
            this->data = nullptr;
            this->n1 = nullptr;
            this->n2 = nullptr;
        }

        void discardData() {
            if (this->ownsData) {
                delete this->data;
                this->data = nullptr;
                this->ownsData = false;
            }
        }

        EdgeId id;
        NodeT *n1, *n2;
        EdgeData *data;
        bool ownsData;
    };
}

#endif //ANDREIUTILS_EDGE_HPP
