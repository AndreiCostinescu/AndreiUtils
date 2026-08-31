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
// Created by Andrei Costinescu (andrei.costinescu@tum.de) on 18.03.2021.
//

#ifndef ANDREIUTILS_CLASSES_PRIORITYQUEUE_HPP
#define ANDREIUTILS_CLASSES_PRIORITYQUEUE_HPP

#include <AndreiUtils/classes/CircularArray.hpp>
#include <AndreiUtils/utils.hpp>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

namespace AndreiUtils {
    template<class T, class P = int>
    class PriorityQueue {
    public:
        explicit PriorityQueue() : PriorityQueue(0) {}

        explicit PriorityQueue(std::vector<T> elems, std::vector<P> priorities) : PriorityQueue(elems.size()) {
            this->add(std::move(elems), std::move(priorities));
        }

        explicit PriorityQueue(std::vector<std::pair<T, P>> values) : PriorityQueue(values.size()) {
            this->add(std::move(values));
        }

        void add(std::vector<T> elems, std::vector<P> priorities) {
            assert(elems.size() == priorities.size());
            for (int i = 0; i < elems.size(); ++i) {
                this->_add(std::move(elems[i]), std::move(priorities[i]), false);
            }
            for (int i = this->content / 2; i >= 0; --i) {
                this->siftDown(i);
            }
        }

        void add(std::vector<std::pair<T, P>> values) {
            for (auto &val: values) {
                this->_add(std::move(val.first), std::move(val.second), false);
            }
            for (int i = this->content / 2; i >= 0; --i) {
                this->siftDown(i);
            }
        }

        int add(T elem, P priority) { return this->_add(std::move(elem), std::move(priority)); }

        T const &first() const {
            if (this->content < 0) {
                throw std::runtime_error("Priority queue is empty");
            }
            return mapGet(this->data, this->indices[0].first);
        }

        T &first() {
            if (this->content < 0) {
                throw std::runtime_error("Priority queue is empty");
            }
            return mapGet(this->data, this->indices[0].first);
        }

        T first(int &id) {
            if (this->content < 0) {
                throw std::runtime_error("Priority queue is empty");
            }
            id = this->indices[0].first;
            return mapGet(this->data, id);
        }

        void removeElement(T const &elem) {
            for (auto const &mapValue: this->data) {
                if (mapValue.second == elem) {
                    this->removeID(mapValue.first);
                    break;
                }
            }
        }

        T removeID(int id) {
            if (!mapContains(this->data, id)) {
                std::cout << "In removeID error..." << std::endl;
                throw std::runtime_error("Priority queue data does not contain the id " + std::to_string(id));
            }
            return this->removeIndex(mapGet(this->positions, id));
        }

        T removeIndex(int index) {
            if (index >= this->content) {
                std::cout << "In removeIndex error..." << std::endl;
                throw std::runtime_error("Priority queue data does not contain the index " + std::to_string(index));
            }
            // std::cout << "Entering PriorityQueue::removeIndex(" << index << ")" << std::endl;

            int id = this->indices[index].first;
            // std::cout << "PriorityQueue::removeIndex(" << index << "): id = " << id << std::endl;
            // Add index to freeIndices
            this->freeIndices.pushBack(id);
            T elem = std::move(mapGet(this->data, id)); // Don't get a reference; get the element itself!
            // std::cout << "PriorityQueue::removeIndex(" << index << "): data.size() = " << this->data.size() <<
            // std::endl;
            this->data.erase(id);
            // std::cout << "PriorityQueue::removeIndex(" << index << "): data.size() = " << this->data.size() <<
            // std::endl; Reconstruct elementIDs array with the remaining data IDs
            this->elementIDs.resize(this->data.size());
            id = 0; // <- just use as counter/loop variable (as opposed of creating another variable just for that)
            for (auto const &x: this->data) {
                /*
                std::cout << "PriorityQueue::removeIndex(" << index << "): re-add " << x.first << " to elementIDs"
                          << std::endl;
                //*/
                this->elementIDs[id++] = x.first;
            }
            assert(id == this->data.size());
            --this->content;
            // this->printContents();
            // "remove from indices array"
            this->swap(this->content, index);
            this->siftDown(index);
            // this->printContents();
            return elem;
        }

        std::pair<T, P> extractMin() {
            T const elem = this->first();
            P const priority = this->indices[0].second;
            this->removeIndex(0);
            return std::make_pair(elem, priority);
        }

        std::pair<T, P> peak() const { return std::make_pair(this->first(), this->indices[0].second); }

        void changePriorityOfElement(T const &elem, P priority) {
            for (auto const &mapValue: this->data) {
                if (mapValue.second == elem) {
                    this->changePriorityOfID(mapValue.first, std::move(priority));
                    break;
                }
            }
        }

        void changePriorityOfID(int elemID, P priority) {
            if (!mapContains(this->data, elemID)) {
                return;
            }
            int const &pos = mapGet(this->positions, elemID);
            this->indices[pos].second = std::move(priority);
            int position = this->siftUp(pos);
            this->siftDown(position);
        }

        int getContentSize() const { return this->content; }

        T const &operator[](int index) const {
            if (index >= this->content) {
                std::cout << "In []-operator error..." << std::endl;
                throw std::runtime_error("Index " + std::to_string(index) +
                                         " is greater than the size of priority queue (" +
                                         std::to_string(this->content) + ")!");
            }
            return mapGet(this->data, this->indices[index].first);
        }

        T &operator[](int index) {
            if (index >= this->content) {
                std::cout << "In []-operator error..." << std::endl;
                throw std::runtime_error("Index " + std::to_string(index) +
                                         " is greater than the size of priority queue (" +
                                         std::to_string(this->content) + ")!");
            }
            return mapGet(this->data, this->indices[index].first);
        }

        T const &operator()(int id) const {
            T const *elem;
            if (!mapGetIfContains(this->data, id, elem)) {
                std::cout << "In ()-operator error..." << std::endl;
                throw std::runtime_error("Element with id " + std::to_string(id) +
                                         " is not part of the priority queue!");
            }
            return *elem;
        }

        T &operator()(int id) {
            T *elem;
            if (!mapGetIfContains(this->data, id, elem)) {
                std::cout << "In ()-operator error..." << std::endl;
                throw std::runtime_error("Element with id " + std::to_string(id) +
                                         " is not part of the priority queue!");
            }
            return *elem;
        }

        P const &priority(int id) const {
            T elem;
            if (!mapGetIfContains(this->data, id, elem)) {
                std::cout << "In priority function error..." << std::endl;
                throw std::runtime_error("Element with id " + std::to_string(id) +
                                         " is not part of the priority queue!");
            }
            return this->indices[mapGet(this->positions, id)].second;
        }

        int getIDOfIndex(int index) const { return this->indices[index].first; }

        void printContents() const {
            std::cout << std::endl;
            std::cout << "Values: ";
            for (int i = 0; i < this->content; ++i) {
                std::cout << mapGet(this->data, this->indices[i].first) << " ";
            }
            std::cout << std::endl;
            std::cout << "Priorities: ";
            for (int i = 0; i < this->content; ++i) {
                std::cout << this->indices[i].second << " ";
            }
            std::cout << std::endl;
            std::cout << "Indices: ";
            for (int i = 0; i < this->content; ++i) {
                std::cout << mapGet(this->positions, this->indices[i].first) << " ";
            }
            std::cout << std::endl;
            std::cout << "IDs: ";
            for (int i = 0; i < this->content; ++i) {
                std::cout << this->indices[i].first << " ";
            }
            std::cout << std::endl;
            std::cout << "Element IDs: ";
            for (int i = 0; i < this->elementIDs.size(); ++i) {
                std::cout << this->elementIDs[i] << " ";
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }

        [[nodiscard]] std::vector<int> const &getElementIDs() const { return this->elementIDs; }

        [[nodiscard]] bool empty() const { return this->content == 0; }

    protected:
        explicit PriorityQueue(int size) : size(size), data(), freeIndices(size), indices(), elementIDs(), content(0) {
            for (int i = 0; i < this->size; ++i) {
                this->freeIndices.pushBack(i);
            }
        }

        int _add(T elem, P priority, bool withSiftUp = true) {
            if (this->content == this->size) {
                this->freeIndices.pushBack(this->size);
                this->size += 1;
            }

            // remove index from freeIndices
            int id = this->freeIndices.popFront();
            this->data[id] = std::move(elem);
            this->elementIDs.push_back(id);
            int position = this->content++;
            mapSet(this->positions, id, position);

            auto p = std::make_pair(id, std::move(priority));
            assert(position <= this->indices.size());
            if (position == this->indices.size()) {
                this->indices.emplace_back(p);
            } else {
                this->indices[position] = p;
            }

            if (withSiftUp) {
                this->siftUp(position);
            }

            return id;
        }

        void swap(int index1, int index2) {
            swapData(this->indices[index1], this->indices[index2]);
            mapSet(this->positions, this->indices[index1].first, index1);
            mapSet(this->positions, this->indices[index2].first, index2);
        }

        int siftUp(int index) {
            while (index > 0) {
                if (less(this->indices[index].second, this->indices[parent(index)].second)) {
                    this->swap(index, parent(index));
                } else {
                    break;
                }
                index = parent(index);
            }
            return index;
        }

        int siftDown(int index) {
            int swapIndex = index;
            int tryIndex = left(index);
            if (tryIndex < this->content && less(this->indices[tryIndex].second, this->indices[swapIndex].second)) {
                swapIndex = tryIndex;
            }
            ++tryIndex; // tryIndex = right(index);
            if (tryIndex < this->content && less(this->indices[tryIndex].second, this->indices[swapIndex].second)) {
                swapIndex = tryIndex;
            }
            if (swapIndex != index) {
                this->swap(index, swapIndex);
                return this->siftDown(swapIndex);
            }
            return index;
        }

        int left(int index) const { return 2 * index + 1; }

        int right(int index) const { return 2 * index + 2; }

        int parent(int index) const { return (index - 1) / 2; }

        int size, content;
        std::vector<int> elementIDs;
        std::vector<std::pair<int, P>> indices; // tuple<data-index and priority> indexed by vector-index
        // std::map<int, bool> freeIndices;  // map of free data-indices (bool is always true for things in map)
        CircularArray<int> freeIndices; // vector of free data-indices
        std::map<int, T> data; // data-index to data
        std::map<int, int> positions; // data-index to vector-index
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_CLASSES_PRIORITYQUEUE_HPP
