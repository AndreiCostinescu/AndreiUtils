//
// Created by Andrei on 23.05.22.
//

#ifndef ANDREIUTILS_UNIONFIND_HPP
#define ANDREIUTILS_UNIONFIND_HPP

#include<AndreiUtils/utilsMap.hpp>
#include<map>
#include<vector>

namespace AndreiUtils {
    template<typename T>
    class UnionFind {
    public:
        explicit UnionFind(int n = 0) : parents(n) {
            this->nrDistinctComponents = n;
            for (int i = 0; i < n; i++) {
                this->parents[i] = i;
            }
        }

        explicit UnionFind(const std::vector<T> &elements) : parents(elements.size()) {
            int n = elements.size();
            this->nrDistinctComponents = n;
            for (int i = 0; i < n; i++) {
                this->idToElement[i] = elements[i];
                this->parents[i] = i;
            }
        }

        int size() const {
            return this->parents.size();
        }

        int numberOfDistinctComponents() const {
            return this->nrDistinctComponents;
        }

        void add(const T &element) {
            // the size of the union-find data structure is always in parents.size()
            int n = this->size();
            this->parents.push_back(n);
            this->sizes.push_back(1);
            this->idToElement[n] = element;
            this->nrDistinctComponents++;
        }

        int find(int id) {
            if (id >= this->size()) {
                throw std::runtime_error(
                        "Index " + std::to_string(id) + " out of bounds (" + std::to_string(this->size()) + ")!");
            }
            if (this->parents[id] != id) {
                this->parents[id] = find(this->parents[id]);
            }
            return this->parents[id];

            // chase parent of current element until it reaches root.
            while (this->parents[id] != id) {
                this->parents[id] = this->parents[this->parents[id]];
                id = this->parents[id];
            }
            return id;
        }

        bool find(int id1, int id2) {
            return (find(id1) == find(id2));
        }

        void unite(int id1, int id2) {
            int root1 = find(id1);
            int root2 = find(id2);
            if (root1 == root2) {
                return;
            }

            if (this->sizes[root1] < this->sizes[root2]) {
                this->parents[root1] = root2;
                this->sizes[root2] += this->sizes[root1];
            } else {
                this->parents[root2] = root1;
                this->sizes[root1] += this->sizes[root2];
            }
            this->nrDistinctComponents--;
        }

        T &get(int id) const {
            return AndreiUtils::mapGetRef(this->idToElement, id);
        }

    private:
        std::map<int, T> idToElement;
        std::vector<int> parents;
        std::vector<int> sizes;
        int nrDistinctComponents{};
    };
}

#endif //ANDREIUTILS_UNIONFIND_HPP
