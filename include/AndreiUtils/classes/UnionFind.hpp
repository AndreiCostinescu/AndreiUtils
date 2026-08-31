// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 23.05.22.
//

#ifndef ANDREIUTILS_UNIONFIND_HPP
#define ANDREIUTILS_UNIONFIND_HPP

#include <AndreiUtils/classes/UnionFind.h>
#include <AndreiUtils/utilsMap.hpp>
#include <functional>
#include <map>

namespace AndreiUtils {
    template<typename T, typename C = std::less<T>, typename A = std::allocator<std::pair<const T, size_t>>>
    class UnionFindWithValues : public UnionFind {
    public:
        explicit UnionFindWithValues(size_t n = 0) :
            UnionFind(n), valueUpdater([](size_t, size_t, size_t, UnionFindWithValues<T, C, A> *) {}) {}

        explicit UnionFindWithValues(const std::vector<T> &elements) : UnionFindWithValues() {
            size_t n = elements.size();
            this->parents.resize(n);
            this->sizes.resize(n);
            this->nrDistinctComponents = n;
            for (size_t i = 0; i < n; i++) {
                this->idToElement[i] = elements[i];
                this->elementToId[elements[i]] = i;
                this->parents[i] = i;
                this->sizes[i] = 1;
            }
        }

        explicit UnionFindWithValues(
                const std::function<void(size_t, size_t, size_t, UnionFindWithValues<T, C, A> *)> &valueUpdater) :
            UnionFind(), valueUpdater(valueUpdater) {}

        void addElem(const T &element) {
            size_t n = this->size();
            this->add();
            this->idToElement[n] = element;
            this->elementToId[element] = n;
        }

        size_t findByElem(const T &elem) { return this->find(this->getId(elem)); }

        bool findByElem(const T &elem1, const T &elem2) { return (findByElem(elem1) == findByElem(elem2)); }

        bool uniteByElem(const T &elem1, const T &elem2) {
            size_t root1 = findByElem(elem1);
            size_t root2 = findByElem(elem2);
            return this->uniteImpl(root1, root2);
        }

        T &get(size_t id) const { return AndreiUtils::mapGet(this->idToElement, id); }

        size_t getId(const T &elem) const { return AndreiUtils::mapGet(this->elementToId, elem); }

        const std::map<size_t, T> &getIdToElement() const { return this->idToElement; }

        const std::map<T, size_t, C, A> &getElementToId() const { return this->elementToId; }

        size_t getComponentSizeByElem(const T &elem) { return this->getComponentSize(this->getId(elem)); }

    protected:
        void setParentIndex(size_t index, size_t parentIndex) override {
            size_t previousParentIndex = this->parents[index];
            this->valueUpdater(index, parentIndex, previousParentIndex, this);
            this->parents[index] = parentIndex;
        }

        std::function<void(size_t, size_t, size_t, UnionFindWithValues<T, C, A> *)> valueUpdater;
        std::map<size_t, T> idToElement;
        std::map<T, size_t, C, A> elementToId;
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_UNIONFIND_HPP
