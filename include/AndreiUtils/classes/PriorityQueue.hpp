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
    template<class T, class P>
    class PriorityQueue {
    public:
        explicit PriorityQueue() : PriorityQueue(0) {}

        explicit PriorityQueue(std::vector<T> elems, std::vector<P> priorities) : PriorityQueue(elems.size()) {
            this->add(elems, priorities);
        }

        explicit PriorityQueue(std::vector<std::pair<T, P>> values) : PriorityQueue(values.size()) {
            this->add(values);
        }

        void add(std::vector<T> elems, std::vector<P> priorities) {
            assert(elems.size() <= priorities.size());
            for (int i = 0; i < elems.size(); i++) {
                this->_add(elems[i], priorities[i], false);
            }
            for (int i = this->content / 2; i >= 0; i--) {
                this->siftDown(i);
            }
        }

        void add(std::vector<std::pair<T, P>> values) {
            for (const auto &val : values) {
                this->_add(val.first, val.second, false);
            }
            for (int i = this->content / 2; i >= 0; i--) {
                this->siftDown(i);
            }
        }

        int add(T elem, const P &priority) {
            return this->_add(elem, priority);
        }

        T first() {
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

        void removeElement(T elem) {
            for (const auto &mapValue : this->data) {
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
            T elem = mapGet(this->data, id);
            // std::cout << "PriorityQueue::removeIndex(" << index << "): data.size() = " << this->data.size() << std::endl;
            this->data.erase(id);
            // std::cout << "PriorityQueue::removeIndex(" << index << "): data.size() = " << this->data.size() << std::endl;
            // Reconstruct elementIDs array with the remaining data IDs
            this->elementIDs.resize(this->data.size());
            id = 0;  // <- just use as counter/loop variable (as opposed of creating another variable just for that)
            for (const auto &x : this->data) {
                /*
                std::cout << "PriorityQueue::removeIndex(" << index << "): re-add " << x.first << " to elementIDs"
                          << std::endl;
                //*/
                this->elementIDs[id++] = x.first;
            }
            assert(id == this->data.size());
            this->content--;
            // this->printContents();
            // "remove from indices array"
            this->swap(this->content, index);
            this->siftDown(index);
            // this->printContents();
            return elem;
        }

        std::pair<T, P> extractMin() {
            T elem = this->first();
            P priority = this->indices[0].second;
            this->removeIndex(0);
            return std::make_pair(elem, priority);
        }

        std::pair<T, P> peak() {
            return std::make_pair(this->first(), this->indices[0].second);
        }

        void changePriorityOfElement(T elem, P priority) {
            for (const auto &mapValue : this->data) {
                if (mapValue.second == elem) {
                    this->changePriorityOfID(mapValue.first, priority);
                    break;
                }
            }
        }

        void changePriorityOfID(int elemID, P priority) {
            if (!mapContains(this->data, elemID)) {
                return;
            }
            int pos = this->positions[elemID];
            this->indices[pos].second = priority;
            int position = this->siftUp(pos);
            this->siftDown(position);
        }

        int getContentSize() {
            return this->content;
        }

        T operator[](int index) {
            if (index >= this->content) {
                std::cout << "In []-operator error..." << std::endl;
                throw std::runtime_error(
                        "Index " + std::to_string(index) + " is greater than the size of priority queue (" +
                        std::to_string(this->content) + ")!");
            }
            return mapGet(this->data, this->indices[index].first);
        }

        T operator()(int id) {
            T elem;
            if (!mapGetIfContains(this->data, id, elem)) {
                std::cout << "In ()-operator error..." << std::endl;
                throw std::runtime_error(
                        "Element with id " + std::to_string(id) + " is not part of the priority queue!");
            }
            return elem;
        }

        P priority(int id) {
            T elem;
            if (!mapGetIfContains(this->data, id, elem)) {
                std::cout << "In priority function error..." << std::endl;
                throw std::runtime_error(
                        "Element with id " + std::to_string(id) + " is not part of the priority queue!");
            }
            return this->indices[this->positions[id]].second;
        }

        int getIDOfIndex(int index) {
            return this->indices[index].first;
        }

        void printContents() {
            std::cout << std::endl;
            std::cout << "Values: ";
            for (int i = 0; i < this->content; i++) {
                std::cout << mapGet(this->data, this->indices[i].first) << " ";
            }
            std::cout << std::endl;
            std::cout << "Priorities: ";
            for (int i = 0; i < this->content; i++) {
                std::cout << this->indices[i].second << " ";
            }
            std::cout << std::endl;
            std::cout << "Indices: ";
            for (int i = 0; i < this->content; i++) {
                std::cout << mapGet(this->positions, this->indices[i].first) << " ";
            }
            std::cout << std::endl;
            std::cout << "IDs: ";
            for (int i = 0; i < this->content; i++) {
                std::cout << this->indices[i].first << " ";
            }
            std::cout << std::endl;
            std::cout << "Element IDs: ";
            for (int i = 0; i < this->elementIDs.size(); i++) {
                std::cout << this->elementIDs[i] << " ";
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }

        [[nodiscard]] const std::vector<int> &getElementIDs() const {
            return this->elementIDs;
        }

        [[nodiscard]] bool empty() const {
            return this->content == 0;
        }

    private:
        explicit PriorityQueue(int size) : size(size), data(), freeIndices(size), indices(), elementIDs(), content(0) {
            for (int i = 0; i < this->size; i++) {
                this->freeIndices.pushBack(i);
            }
        }

        int _add(T elem, const P &priority, bool withSiftUp = true) {
            if (this->content == this->size) {
                this->freeIndices.pushBack(this->size);
                this->size += 1;
            }

            // remove index from freeIndices
            int id = this->freeIndices.popFront();
            this->data[id] = elem;
            this->elementIDs.push_back(id);
            this->content++;
            int position = this->content - 1;
            this->positions[id] = position;

            auto p = std::make_pair(id, priority);
            assert(position <= this->indices.size());
            if (position == this->indices.size()) {
                this->indices.push_back(p);
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
            this->positions[this->indices[index1].first] = index1;
            this->positions[this->indices[index2].first] = index2;
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
            tryIndex++;  // tryIndex = right(index);
            if (tryIndex < this->content && less(this->indices[tryIndex].second, this->indices[swapIndex].second)) {
                swapIndex = tryIndex;
            }
            if (swapIndex != index) {
                this->swap(index, swapIndex);
                return this->siftDown(swapIndex);
            }
            return index;
        }

        int left(int index) {
            return 2 * index + 1;
        }

        int right(int index) {
            return 2 * index + 2;
        }

        int parent(int index) {
            return (index - 1) / 2;
        }

        int size, content;
        std::vector<int> elementIDs;
        std::vector<std::pair<int, P>> indices;  // tuple<data-index and priority> indexed by vector-index
        // std::map<int, bool> freeIndices;  // map of free data-indices (bool is always true for things in map)
        CircularArray<int> freeIndices;  // vector of free data-indices
        std::map<int, T> data;  // data-index to data
        std::map<int, int> positions;  // data-index to vector-index
    };
}

#endif //ANDREIUTILS_CLASSES_PRIORITYQUEUE_HPP
