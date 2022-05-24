//
// Created by Andrei on 23.05.22.
//

#ifndef ANDREIUTILS_UNIONFIND_HPP
#define ANDREIUTILS_UNIONFIND_HPP

#include<AndreiUtils/classes/UnionFind.h>
#include<AndreiUtils/utilsMap.hpp>
#include<map>

namespace AndreiUtils {
    template<typename T, typename C = std::less<T>, typename A = std::allocator<std::pair<const T, size_t>>>
    class UnionFindWithValues : public UnionFind{
    public:
        explicit UnionFindWithValues(size_t n = 0) : UnionFind(n) {}

        explicit UnionFindWithValues(const std::vector<T> &elements) : UnionFind() {
            size_t n = elements.size();
            this->parents.resize(n);
            this->nrDistinctComponents = n;
            for (size_t i = 0; i < n; i++) {
                this->idToElement[i] = elements[i];
                this->elementToId[elements[i]] = i;
                this->parents[i] = i;
                this->sizes[i] = 1;
            }
        }

        void add(const T &element) {
            // the size of the union-find data structure is always in parents.size()
            size_t n = this->size();
            this->parents.push_back(n);
            this->sizes.push_back(1);
            this->idToElement[n] = element;
            this->elementToId[element] = n;
            this->nrDistinctComponents++;
        }

        size_t findByElem(const T &elem) {
            return this->find(mapGet(this->elementToId, elem));
        }

        bool findByElem(const T &elem1, const T &elem2) {
            return (find(elem1) == find(elem2));
        }

        void uniteByElem(const T &elem1, const T &elem2) {
            size_t root1 = find(elem1);
            size_t root2 = find(elem2);
            this->uniteImpl(root1, root2);
        }

        T &get(size_t id) const {
            return AndreiUtils::mapGetRef(this->idToElement, id);
        }

    protected:
        std::map<size_t, T> idToElement;
        std::map<T, size_t, C, A> elementToId;
    };
}

#endif //ANDREIUTILS_UNIONFIND_HPP
