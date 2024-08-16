//
// Created by Andrei on 24.03.22.
//

#pragma once

#include <functional>
#include <iostream>
#include <sstream>
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
    void printMatrix(T const *x, int rows, int cols, std::string const &separator = ", ", bool withNewline = true) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (j > 0) {
                    std::cout << separator;
                }
                std::cout << x[i * cols + j];
            }
            std::cout << std::endl;
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T>
    void printMatrix(T const *x, int rows, int cols, std::function<std::string(T const &)> const &stringConversion,
                     std::string const &separator = ", ", bool withNewline = true) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (j > 0) {
                    std::cout << separator;
                }
                std::cout << stringConversion(x[i * cols + j]);
            }
            std::cout << std::endl;
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T>
    void printMatrix(T const *x, int rows, int cols,
                     std::function<std::string(T const &, int const &, int const &)> const &stringConversion,
                     std::string const &separator = ", ", bool withNewline = true) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (j > 0) {
                    std::cout << separator;
                }
                std::cout << stringConversion(x[i * cols + j], i, j);
            }
            std::cout << std::endl;
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T>
    std::string printMatrixToString(T const *x, int rows, int cols, std::string const &separator = ", ") {
        std::stringstream s;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (j > 0) {
                    s << separator;
                }
                s << x[i * cols + j];
            }
            s << std::endl;
        }
        return s.str();
    }

    template<class T>
    std::string printMatrixToString(T const *x, int rows, int cols,
                                    std::function<std::string(T const &)> const &stringConversion,
                                    std::string const &separator = ", ") {
        std::stringstream s;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (j > 0) {
                    s << separator;
                }
                s << stringConversion(x[i * cols + j]);
            }
            s << std::endl;
        }
        return s.str();
    }

    template<class T>
    std::string printMatrixToString(
            T const *x, int rows, int cols,
            std::function<std::string(T const &, int const &, int const &)> const &stringConversion,
            std::string const &separator = ", ") {
        std::stringstream s;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (j > 0) {
                    s << separator;
                }
                s << stringConversion(x[i * cols + j], i, j);
            }
            s << std::endl;
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
