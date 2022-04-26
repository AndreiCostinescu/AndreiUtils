//
// Created by andrei on 24.03.22.
//

#ifndef ANDREIUTILS_UTILSVECTOR_HPP
#define ANDREIUTILS_UTILSVECTOR_HPP

#include <algorithm>
#include <AndreiUtils/utils.hpp>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

namespace AndreiUtils {
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

    template<class T>
    void forLoop(const std::vector<T> &array, size_t size,
                 std::function<void(const std::vector<T> &, size_t, size_t)> op, size_t increment = 1) {
        for (size_t i = 0; i < size; i += increment) {
            op(array, i, increment);
        }
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

    template<class T>
    void printVector(const T *&x, int size) {
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << x[i];
        }
        std::cout << std::endl;
    }

    template<class T>
    void printVector(const T *&x, int size, const std::function<std::string(T const &)> &stringConversion) {
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << stringConversion(x[i]);
        }
        std::cout << std::endl;
    }

    template<class T>
    std::string printVectorToString(const T *&x, int size) {
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
    std::string printVectorToString(const T *&x, int size,
                                    const std::function<std::string(T const &)> &stringConversion) {
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
    std::vector<T> mergeVectors(const std::vector<T> &v1, const std::vector<T> &v2) {
        std::vector<T> merged(v1);
        merged.insert(merged.end(), v2.begin(), v2.end());
        return merged;
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
                swapData(v[oldJ], v[j]);
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
}

#endif //ANDREIUTILS_UTILSVECTOR_HPP
