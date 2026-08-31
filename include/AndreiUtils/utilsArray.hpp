/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
    std::string
    printMatrixToString(T const *x, int rows, int cols,
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
} // namespace AndreiUtils
