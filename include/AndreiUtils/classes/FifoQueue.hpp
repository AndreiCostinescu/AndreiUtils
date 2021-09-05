//
// Created by Andrei Costinescu (andrei.costinescu@tum.de) on 26.03.2021.
//

#ifndef ANDREIUTILS_CLASSES_FIFOQUEUE_HPP
#define ANDREIUTILS_CLASSES_FIFOQUEUE_HPP

#include <cstring>  // for memcpy
#include <stdexcept>  // for runtime_error

namespace AndreiUtils {
    template<class T>
    class FifoQueue {
    public:
        class FifoQueueIterator : public std::iterator<std::output_iterator_tag, T> {
        public:
            explicit FifoQueueIterator(const FifoQueue<T> *data, size_t index = 0) : data(data), index(index) {}

            T &operator*() const {
                return (*(this->data))[this->index];
            }

            FifoQueueIterator &operator++() {
                ++this->index;
                return *this;
            }

            const FifoQueueIterator operator++(int inc) {  // // NOLINT(readability-const-return-type)
                this->index += inc;
                return FifoQueueIterator(*this);
            }

            bool operator==(const FifoQueueIterator &rhs) const {
                return this->index == rhs.index;
            }

            bool operator!=(const FifoQueueIterator &rhs) const {
                return this->index != rhs.index;
            }

        private:
            size_t index;
            const FifoQueue<T> *data;
        };

        explicit FifoQueue(unsigned int size, bool fixSize = false)
                : data(nullptr), size(0), contentSize(0), contentStart(0), contentEnd(0), fixSize(fixSize) {
            this->resize(size);
        }

        virtual ~FifoQueue() {
            this->reset();
        }

        void reset() {
            delete[] this->data;
            this->data = nullptr;
            this->size = 0;
            this->contentSize = 0;
            this->contentStart = 0;
            this->contentEnd = 0;
        }

        FifoQueue::FifoQueueIterator begin() const {
            return FifoQueue::FifoQueueIterator(this, 0);
        }

        FifoQueue::FifoQueueIterator end() const {
            return FifoQueue::FifoQueueIterator(this, this->contentSize);
        }

        T pop() {
            if (this->contentSize == 0) {
                throw std::runtime_error("FifoQueue is empty... Can't pop!");
            }
            // think of dynamic arrays and make */ 2
            T returnVal = this->data[this->contentStart];
            this->contentStart = (this->contentStart + 1) % this->size;
            this->contentSize--;
            return returnVal;
        }

        void push(T elem) {
            if (this->contentSize >= this->size) {
                if (this->fixSize) {
                    throw std::runtime_error("FifoQueue is created with fixed size and the queue is full... "
                                             "Can't push element");
                } else {
                    this->resize(2 * this->size);  // think of dynamic arrays and make */ 2
                }
            }
            this->data[this->contentEnd] = elem;
            this->contentEnd = (this->contentEnd + 1) % this->size;
            this->contentSize++;
        }

        [[nodiscard]] int getContentSize() const {
            return this->contentSize;
        }

        [[nodiscard]] T &getFirst() const {
            if (this->contentSize == 0) {
                throw std::runtime_error("FifoQueue is empty... Can't get first element!");
            }
            return this->data[this->contentStart];
        }

        [[nodiscard]] bool isFull() const {
            return this->size == this->contentSize;
        }

        [[nodiscard]] bool empty() const {
            return this->contentSize == 0;
        }

        T &operator[](unsigned int index) const {
            if (index >= this->contentSize) {
                throw std::out_of_range("Index larger than size in FifoQueue::[]");
            }
            index += this->contentStart;
            if (index >= this->size) {
                index -= this->size;
            }
            return this->data[index];
        }

    private:
        void resize(int newSize) {
            if (newSize < this->size) {
                return;
            }

            T *tmpData = this->data;
            this->data = new T[newSize];
            if (tmpData != nullptr) {
                if (this->contentStart <= this->contentEnd) {
                    if (this->contentSize > 0) {
                        memcpy(this->data, tmpData + this->contentStart, sizeof(T) * (this->contentSize));
                    }
                } else {
                    unsigned int firstPart = this->size - this->contentStart;
                    memcpy(this->data, tmpData + this->contentStart, sizeof(T) * firstPart);
                    memcpy(this->data + firstPart, tmpData, sizeof(T) * (this->contentSize - firstPart));
                }
                delete[] tmpData;
            }
            this->size = newSize;
            this->contentStart = 0;
            this->contentEnd = this->contentSize % this->size;  // but this should always be contentSize...
        }

        T *data;
        unsigned int size, contentSize, contentStart, contentEnd;
        bool fixSize;
    };
}

#endif //ANDREIUTILS_CLASSES_FIFOQUEUE_HPP
