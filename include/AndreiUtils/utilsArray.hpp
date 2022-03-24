//
// Created by andrei on 24.03.22.
//

#ifndef ANDREIUTILS_UTILSARRAY_HPP
#define ANDREIUTILS_UTILSARRAY_HPP

#include <functional>
#include <iostream>
#include <string>

namespace AndreiUtils {
    template<typename T>
    inline T average(T *a, int start, int size) {
        T avg;
        if (size == 0) {
            return avg;
        }
        for (int i = start; i < start + size; i++) {
            avg += a[i];
        }
        avg /= size;
        return avg;
    }

    template<class T>
    void printMatrix(T *x, int rows, int cols) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                std::cout << x[i * cols + j] << ", ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    template<class T>
    void printVector(T *x, int size) {
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << x[i];
        }
        std::cout << std::endl;
    }

    template<class T>
    std::string printVectorToString(T *x, int size) {
        std::stringstream s;
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                s << ", ";
            }
            s << x[i];
        }
        return s.str();
    }

    template<class T>
    void printVector(T *x, int size, const std::function<std::string(T const &)> &stringConversion) {
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << stringConversion(x[i]);
        }
        std::cout << std::endl;
    }

    template<class T>
    std::string printVectorToString(T *x, int size, const std::function<std::string(T const &)> &stringConversion) {
        std::stringstream s;
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                s << ", ";
            }
            s << stringConversion(x[i]);
        }
        return s.str();
    }

    template<class T>
    void forLoop(T *const array, size_t size, std::function<void(T *const, size_t, size_t)> op, size_t increment = 1) {
        for (size_t i = 0; i < size; i += increment) {
            op(array, i, increment);
        }
    }
}

#endif //ANDREIUTILS_UTILSARRAY_HPP
