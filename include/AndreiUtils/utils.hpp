//
// Created by Andrei on 13.11.20.
//

#ifndef ANDREIUTILS_UTILS_HPP
#define ANDREIUTILS_UTILS_HPP

#include <algorithm>
#include <cassert>
#include <cstring>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
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
    inline T fastAbs(const T x) {
        return (less<T>(x, 0) ? -x : x);
    }

    template<typename T>
    inline bool equal(const T a, const T b, double tol = 1e-9) {
        return (fastAbs(a - b) < tol);
    }

    template<typename T>
    inline T fastMin(const T a, const T b) {
        return (less<T>(a, b) ? a : b);
    }

    template<typename T>
    inline T fastMax(const T a, const T b) {
        return (greater<T>(a, b) ? a : b);
    }

    // Don't have 'a' be a (const) reference, because std::nth_element changes the data inside 'a'!
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
    inline T average(const std::vector<T> &a) {
        T avg;
        if (a.empty()) {
            return avg;
        }
        for (const T &val: a) {
            avg += val;
        }
        avg /= (double) a.size();
        return avg;
    }

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

    /*
    template<class T>
    bool vectorContains(const std::vector<T> &container, const T &key) {
        for (const auto &value: container) {
            if (key == value) {
                return true;
            }
        }
        return false;
    }
    //*/

    template<class T>
    bool vectorContains(const std::vector<T> &container, const T &key, std::size_t *position = nullptr) {
        auto iter = find(container.begin(), container.end(), key);
        if (iter == container.end()) {
            return false;
        }
        if (position != nullptr) {
            *position = iter - container.begin();
        }
        return true;
    }

    template<class T>
    bool vectorContains(const std::vector<T *> &container, T *&key, std::size_t *position = nullptr) {
        auto iter = find(container.begin(), container.end(), key);
        if (iter == container.end()) {
            return false;
        }
        if (position != nullptr) {
            *position = iter - container.begin();
        }
        return true;
    }

    template<class T>
    bool vectorContains(const std::vector<T *> &container, T *const &key, std::size_t *position = nullptr) {
        auto iter = find(container.begin(), container.end(), key);
        if (iter == container.end()) {
            return false;
        }
        if (position != nullptr) {
            *position = iter - container.begin();
        }
        return true;
    }

    template<class T>
    bool vectorContains(const std::vector<T *> &container, T const *&key, std::size_t *position = nullptr) {
        auto iter = find(container.begin(), container.end(), key);
        if (iter == container.end()) {
            return false;
        }
        if (position != nullptr) {
            *position = iter - container.begin();
        }
        return true;
    }

    template<class T>
    bool vectorContains(const std::vector<T *> &container, T const *const &key, std::size_t *position = nullptr) {
        auto iter = find(container.begin(), container.end(), key);
        if (iter == container.end()) {
            return false;
        }
        if (position != nullptr) {
            *position = iter - container.begin();
        }
        return true;
    }

    template<class T>
    void vectorRemoveFirstValueMatch(std::vector<T> &container, const T &value) {
        for (auto i = container.begin(); i != container.end(); ++i) {
            if (*i == value) {
                container.erase(i);
                return;
            }
        }
    }

    template<class T>
    void vectorRemoveAllValueMatches(std::vector<T> &container, const T &value) {
        for (auto i = container.begin(); i != container.end(); ++i) {
            if (*i == value) {
                container.erase(i);
                i--;
            }
        }
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

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    bool mapContains(const std::map<T1 *, T2, C, A> &container, const T1 *key, T2 *value = nullptr) {
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

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    bool mapGetIfContains(const std::map<T1 *, T2, C, A> &container, const T1 *key, T2 &value) {
        return mapContains(container, key, &value);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    T2 mapGet(const std::map<T1, T2, C, A> &container, const T1 &key) {
        T2 value;
        if (!mapContains(container, key, &value)) {
            throw std::runtime_error("Element not found in map!");
        }
        return value;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    T2 mapGet(const std::map<T1 *, T2, C, A> &container, const T1 *key) {
        T2 value;
        if (!mapContains(container, key, &value)) {
            throw std::runtime_error("Element not found in map!");
        }
        return value;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    T2 &mapGetRef(const std::map<T1, T2, C, A> &container, const T1 &key) {
        if (!mapContains(container, key)) {
            throw std::runtime_error("Element not found in map!");
        }
        return container[key];
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    T2 &mapGetRef(const std::map<T1 *, T2, C, A> &container, const T1 *key) {
        if (!mapContains(container, key)) {
            throw std::runtime_error("Element not found in map!");
        }
        return container[key];
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void mapSet(const std::map<T1, T2, C, A> &container, const T1 &key, const T2 &value) {
        container[key] = value;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    void mapSet(const std::map<T1 *, T2, C, A> &container, const T1 *key, const T2 &value) {
        container[key] = value;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    bool mapDelete(std::map<T1, T2, C, A> &container, const T1 &key) {
        return (container.erase(key) > 0);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    bool mapDelete(std::map<T1 *, T2, C, A> &container, const T1 *key) {
        return (container.erase(key) > 0);
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
    void printVector(const std::vector<T> &x) {
        for (int i = 0; i < x.size(); i++) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << x[i];
        }
        std::cout << std::endl;
    }

    template<class T>
    std::string printVectorToString(const std::vector<T> &x) {
        std::stringstream s;
        for (int i = 0; i < x.size(); i++) {
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
    void printVector(const std::vector<T> &x, const std::function<std::string(T const &)> &stringConversion) {
        for (int i = 0; i < x.size(); i++) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << stringConversion(x[i]);
        }
        std::cout << std::endl;
    }

    template<class T>
    std::string printVectorToString(const std::vector<T> &x,
                                    const std::function<std::string(T const &)> &stringConversion) {
        std::stringstream s;
        for (int i = 0; i < x.size(); i++) {
            if (i > 0) {
                s << ", ";
            }
            s << stringConversion(x[i]);
        }
        return s.str();
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

    template<typename T, typename Compare>
    std::vector<std::size_t> getSortedIndicesOfVector(const std::vector<T> &v, const Compare &compare) {
        std::vector<std::size_t> indices(v.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(), [&](std::size_t i, std::size_t j) { return compare(v[i], v[j]); });
        return indices;
    }

    template<typename T>
    std::vector<T> permuteVector(const std::vector<T> &v, const std::vector<std::size_t> &permutationIndices) {
        std::vector<T> permutation(v.size());
        std::transform(permutationIndices.begin(), permutationIndices.end(), permutation.begin(),
                       [&](std::size_t i) { return v[i]; });
        return permutation;
    }

    template<typename T>
    void permuteVectorInPlace(std::vector<T> &v, const std::vector<std::size_t> &permutationIndices) {
        std::vector<int> done(v.size(), 0);
        for (std::size_t i = 0; i < v.size(); i++) {
            if (done[i] == 1) {
                continue;
            }
            done[i] = 1;
            std::size_t oldJ = i;
            std::size_t j = permutationIndices[i];
            while (i != j) {
                std::swap(v[oldJ], v[j]);
                done[j] = 1;
                oldJ = j;
                j = permutationIndices[j];
            }
        }
    }

    template<typename T>
    void sortMultipleVectorsBasedOnPermutation(const std::vector<std::size_t> &permutationIndices, std::vector<T> &v) {
        permuteVectorInPlace(v, permutationIndices);
    }

    template<typename T1, typename... TArgs>
    void sortMultipleVectorsBasedOnPermutation(const std::vector<std::size_t> &permutationIndices, std::vector<T1> &v1,
                                               TArgs &... v2) {
        permuteVectorInPlace(v1, permutationIndices);
        sortMultipleVectorsBasedOnPermutation(permutationIndices, v2...);
    }

    template<class T1, class T2>
    std::map<T1, T2> createMapFromKeysAndValues(const std::vector<T1> &keys, const std::vector<T2> &values) {
        assert (keys.size() == values.size());
        std::map<T1, T2> res;
        for (int i = 0; i < keys.size(); i++) {
            res[keys[i]] = values[i];
        }
        return res;
    }
}

#endif //ANDREIUTILS_UTILS_HPP
