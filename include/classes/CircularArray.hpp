//
// Created by Andrei Costinescu (andrei.costinescu@tum.de) on 11.05.2021.
//

#ifndef ANDREIUTILS_CLASSES_CIRCULARARRAY_HPP
#define ANDREIUTILS_CLASSES_CIRCULARARRAY_HPP

#include <iostream>
#include <map>
#include <stdexcept>
#include <utils/utils.hpp>
#include <vector>

template<class T>
class CircularArray {
public:
    explicit CircularArray(int initialSize = 0) :
            dataSize(initialSize), data(), startIndex(0), endIndex(0), dataLength(0) {
        if (this->dataSize > 0) {
            this->data = this->createDataContainer(this->dataSize);
        }
    }

    ~CircularArray() {
        this->deleteDataContainer();
    }

    void reserve(int size) {
        if (size > this->dataSize) {
            this->deleteDataContainer();
            this->dataSize = size;
            this->data = this->createDataContainer(this->dataSize);
        }
    }

    [[nodiscard]] bool empty() const {
        return this->dataLength == 0;
    }

    [[nodiscard]] int size() const {
        return this->dataLength;
    }

    void pushBack(T element) {
        if (this->dataLength >= this->dataSize) {
            this->resize();
        }
        *this->data[this->endIndex] = element;
        this->dataLength++;
        this->endIndex = (this->endIndex + 1) % this->dataSize;
    }

    [[nodiscard]] T popFront() {
        if (this->dataLength <= 0) {
            throw std::runtime_error("Can't pop from an empty circular array!");
        }
        T element = *this->data[this->startIndex];
        this->dataLength--;
        this->startIndex = (this->startIndex + 1) % this->dataSize;
        return element;
    }

    [[nodiscard]] T get(int i) const {
        return *this->getPtr(i);
    }

    [[nodiscard]] T operator[](int i) const {
        return this->get(i);
    }

    T &operator()(int &i) {
        i %= this->dataSize;
        return *this->getPtr(i);
    }

private:
    T **createDataContainer(int size, int from = 0) {
        T **newData = new T *[size];
        for (int i = from; i < size; i++) {
            newData[i] = new T;
        }
        return newData;
    }

    void deleteDataContainer() {
        for (int i = 0; i < this->dataSize; i++) {
            delete this->data[i];
        }
        delete[] this->data;
    }

    [[nodiscard]] T *getPtr(int i) const {
        if (i >= this->dataLength) {
            throw std::runtime_error("Can't get element at index higher than the array size");
        }
        T *element = this->data[(this->startIndex + i) % this->dataSize];
        assert (element != nullptr);
        return element;
    }

    void resize() {
        int newSize = 2 * (this->dataSize ? this->dataSize : 1);
        T **newData = this->createDataContainer(newSize, this->dataSize);
        for (int i = 0; i < this->dataLength; i++) {
            newData[i] = this->getPtr(i);
        }
        delete[] this->data;
        this->data = newData;
        this->dataSize = newSize;
        this->startIndex = 0;
        this->endIndex = this->dataLength;
    }

    T **data;
    int dataSize, startIndex, endIndex, dataLength;
};

#endif //ANDREIUTILS_CLASSES_CIRCULARARRAY_HPP
