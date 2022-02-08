//
// Created by Andrei on 13.11.20.
//

#ifndef ANDREIUTILS_UTILS_HPP
#define ANDREIUTILS_UTILS_HPP

#include <algorithm>
#include <cassert>
#include <cstring>
#include <cmath>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace AndreiUtils {
    template<typename T>
    void swap(T *a, T *b) {
        if (a == nullptr || b == nullptr) {
            throw std::runtime_error("Can not swap null pointer data");
        }
        T tmp = *a;
        *a = *b;
        *b = tmp;
    }

    template<typename T>
    void swap(T &a, T &b) {
        swap(&a, &b);
    }

    template<typename T>
    inline int intRound(const T a) {
        return lround(a);
    }

    template<typename T>
    inline T fastMin(const T a, const T b) {
        return (a < b ? a : b);
    }

    template<typename T>
    inline T fastMax(const T a, const T b) {
        return (a > b ? a : b);
    }

    template<typename T>
    inline bool equal(const T a, const T b, double tol = 1e-9) {
        return (abs(a - b) < tol);
    }

    template<typename T>
    inline bool less(const T a, const T b, double tol = 1e-9) {
        return (b - a) > tol;
    }

    template<typename T>
    inline bool lessEqual(const T a, const T b, double tol = 1e-9) {
        return (b - a) > -tol;
    }

    template<typename T>
    inline bool greater(const T a, const T b, double tol = 1e-9) {
        return (a - b) > tol;
    }

    template<typename T>
    inline bool greaterEqual(const T a, const T b, double tol = 1e-9) {
        return (a - b) > -tol;
    }

    template<typename T>
    inline T median(std::vector<T> a) {
        int n = a.size();
        // Applying nth_element on n/2th index
        std::nth_element(a.begin(), a.begin() + n / 2, a.end());
        T median = a[n / 2];

        if (n % 2 == 0) {
            // Applying nth_element on (n-1)/2 th index
            std::nth_element(a.begin(), a.begin() + (n - 1) / 2, a.end());
            median = (a[(n - 1) / 2] + median) / 2.0;
        }
        return median;
    }

    template<typename T>
    inline T average(std::vector<T> a) {
        T avg;
        for (const T &val: a) {
            avg += val;
        }
        avg /= (double) a.size();
        return avg;
    }

    template<typename T>
    inline T average(T *a, int start, int size) {
        T avg;
        for (int i = start; i < start + size; i++) {
            avg += a[i];
        }
        avg /= size;
        return avg;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    bool mapContains(const std::map<T1, T2, C, A> &container, const T1 &key, T2 *value = nullptr) {
        const auto &data = container.find(key);
        if (data != container.end()) {
            if (value != nullptr) {
                *value = data->second;
            }
            return true;
        }
        return false;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    bool mapGetIfContains(const std::map<T1, T2, C, A> &container, const T1 &key, T2 &value) {
        return mapContains(container, key, &value);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    T2 mapGet(const std::map<T1, T2, C, A> &container, const T1 &key) {
        T2 value;
        if (!mapContains(container, key, &value)) {
            assert(false);
        }
        return value;
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
        std::string s;
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                s += ", ";
            }
            s += std::string(x[i]);
        }
        return s;
    }

    template<class T>
    void printVector(std::vector<T> x) {
        for (int i = 0; i < x.size(); i++) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << x[i];
        }
        std::cout << std::endl;
    }

    template<class T>
    std::string printVectorToString(std::vector<T> x) {
        std::string s;
        for (int i = 0; i < x.size(); i++) {
            if (i > 0) {
                s += ", ";
            }
            s += std::string(x[i]);
        }
        return s;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMap(const std::map<T1, T2, C, A> &container) {
        for (const auto &containerItem: container) {
            std::cout << containerItem.first << " -> " << containerItem.second << std::endl;
        }
    }

    template<class T>
    std::vector<T> mergeVectors(const std::vector<T> &v1, const std::vector<T> &v2) {
        std::vector<T> merged(v1);
        merged.insert(merged.end(), v2.begin(), v2.end());
        return merged;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::vector<T1> getMapKeys(const std::map<T1, T2, C, A> &container) {
        std::vector<T1> keys(container.size());
        int i = 0;
        for (const auto &elem: container) {
            keys[i++] = elem.first;
        }
        return keys;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::vector<T2> getMapValues(const std::map<T1, T2, C, A> &container) {
        std::vector<T2> values(container.size());
        int i = 0;
        for (const auto &elem: container) {
            values[i++] = elem.second;
        }
        return values;
    }

    template<class T>
    void forLoop(T *const array, size_t size, std::function<void(T *const, size_t, size_t)> op, size_t increment = 1) {
        for (size_t i = 0; i < size; i += increment) {
            op(array, i, increment);
        }
    }

    template<class T>
    void forLoop(const std::vector<T> &array, size_t size,
                 std::function<void(const std::vector<T> &, size_t, size_t)> op, size_t increment = 1) {
        for (size_t i = 0; i < size; i += increment) {
            op(array, i, increment);
        }
    }
}

#endif //ANDREIUTILS_UTILS_HPP
