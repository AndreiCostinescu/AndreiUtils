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
        if (a.empty()) {
            return T();
        }
        T avg = a[0];
        size_t n = a.size();
        for (size_t i = 1; i < n; i++) {
            avg = avg + a[i];
        }
        avg = avg / ((double) a.size());
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
    bool vectorContains(const std::vector<T> &container, std::function<bool(T const &)> const &predicate,
                        std::size_t *position = nullptr) {
        for (size_t i = 0; i < container.size(); i++) {
            if (predicate(container[i])) {
                if (position != nullptr) {
                    *position = i;
                }
                return true;
            }
        }
        return false;
    }

    template<class T>
    bool vectorContains(const std::vector<T *> &container, std::function<bool(T const *const &)> const &predicate,
                        std::size_t *position = nullptr) {
        for (size_t i = 0; i < container.size(); i++) {
            if (predicate(container[i])) {
                if (position != nullptr) {
                    *position = i;
                }
                return true;
            }
        }
        return false;
    }

    template<class T>
    bool vectorContains(const std::vector<T> &container, const T &key, std::size_t *position = nullptr) {
        auto iter = std::find(container.begin(), container.end(), key);
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
        auto iter = std::find(container.begin(), container.end(), key);
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
        auto iter = std::find(container.begin(), container.end(), key);
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
        auto iter = std::find(container.begin(), container.end(), key);
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
        auto iter = std::find(container.begin(), container.end(), key);
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
    void printVector(const std::vector<T> &x, const std::string &separator = ", ") {
        for (int i = 0; i < x.size(); i++) {
            if (i > 0) {
                std::cout << separator;
            }
            std::cout << x[i];
        }
        std::cout << std::endl;
    }

    template<class T>
    void printVector(const std::vector<T> &x, const std::function<std::string(T const &)> &stringConversion,
                     const std::string &separator = ", ") {
        for (int i = 0; i < x.size(); i++) {
            if (i > 0) {
                std::cout << separator;
            }
            std::cout << stringConversion(x[i]);
        }
        std::cout << std::endl;
    }

    template<class T>
    std::string printVectorToString(const std::vector<T> &x, const std::string &separator = ", ") {
        std::stringstream s;
        for (int i = 0; i < x.size(); i++) {
            if (i > 0) {
                s << separator;
            }
            s << x[i];
        }
        return s.str();
    }

    template<class T>
    std::string printVectorToString(const std::vector<T> &x,
                                    const std::function<std::string(T const &)> &stringConversion,
                                    const std::string &separator = ", ") {
        std::stringstream s;
        for (int i = 0; i < x.size(); i++) {
            if (i > 0) {
                s << separator;
            }
            s << stringConversion(x[i]);
        }
        return s.str();
    }

    template<class T>
    void printVector(const T *const &x, int size, const std::string &separator = ", ") {
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                std::cout << separator;
            }
            std::cout << x[i];
        }
        std::cout << std::endl;
    }

    template<class T>
    void printVector(const T *const &x, int size, const std::function<std::string(T const &)> &stringConversion,
                     const std::string &separator = ", ") {
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                std::cout << separator;
            }
            std::cout << stringConversion(x[i]);
        }
        std::cout << std::endl;
    }

    template<class T>
    std::string printVectorToString(const T *const &x, int size, const std::string &separator = ", ") {
        std::stringstream s;
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                s << separator;
            }
            s << x[i];
        }
        return s.str();
    }

    template<class T>
    std::string printVectorToString(const T *const &x, int size,
                                    const std::function<std::string(T const &)> &stringConversion,
                                    const std::string &separator = ", ") {
        std::stringstream s;
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                s << separator;
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

    template<class T>
    bool isSequenceStable(const std::vector<T> &sequence, const T &avg,
                          const std::function<double(const T &, const T &)> &op, double stabilityThreshold = 0.5,
                          bool verbose = false) {
        double sumOfSquaredDifferences = 0.;
        for (const auto &element: sequence) {
            sumOfSquaredDifferences += op(element, avg);
        }
        if (verbose) {
            std::cout << "Sum of squared differences for relative pose = " << sumOfSquaredDifferences << std::endl;
        }
        return sumOfSquaredDifferences < stabilityThreshold;
    }

    template<class T>
    bool isSequenceStable(const std::vector<T> &sequence, const T &avg, double stabilityThreshold = 0.5,
                          bool verbose = false) {
        return isSequenceStable(sequence, avg, [](const T &t1, const T &t2) { return std::abs(t1 - t2); },
                                stabilityThreshold, verbose);
    }

    template<class T>
    bool isSequenceStable(const std::vector<T> &sequence, double stabilityThreshold = 0.5, bool verbose = false) {
        if (sequence.empty()) {
            return true;
        }
        return isSequenceStable(sequence, average(sequence), stabilityThreshold, verbose);
    }

    template<class T>
    bool isSequenceStable(const std::vector<T> &sequence, const std::function<double(const T &, const T &)> &op,
                          double stabilityThreshold = 0.5, bool verbose = false) {
        if (sequence.empty()) {
            return true;
        }
        return isSequenceStable(sequence, average(sequence), op, stabilityThreshold, verbose);
    }
}

#endif //ANDREIUTILS_UTILSVECTOR_HPP
