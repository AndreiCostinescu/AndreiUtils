//
// Created by Andrei on 24.03.22.
//

#pragma once

#include <algorithm>
#include <AndreiUtils/classes/RandomNumberGenerator.hpp>
#include <AndreiUtils/enums/StabilityCriterionOperation.h>
#include <AndreiUtils/utils.hpp>
#include <cstring>
#include <functional>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <sstream>
#include <vector>

namespace AndreiUtils {
    // Don't have 'a' be a (const) reference, because std::nth_element changes the data inside 'a'!
    template<typename T>
    [[nodiscard]] inline T median(std::vector<T> a) {
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
    [[nodiscard]] inline T average(std::vector<T> const &a) {
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
    void forLoop(std::vector<T> const &array, size_t size,
                 std::function<void(std::vector<T> const &, size_t, size_t)> const &op, size_t increment = 1) {
        for (size_t i = 0; i < size; i += increment) {
            op(array, i, increment);
        }
    }

    /*
    template<class T>
    bool vectorContains(std::vector<T> const &container, const T &key) {
        for (const auto &value: container) {
            if (key == value) {
                return true;
            }
        }
        return false;
    }
    //*/

    template<typename T>
    [[nodiscard]] std::vector<T> vectorFromArray(T const *const &array, size_t size) {
        std::vector<T> res(size);
        for (size_t i = 0; i < size; i++) {
            res[i] = array[i];
        }
        return res;
    }

    template<class T>
    [[nodiscard]] bool vectorContains(std::vector<T> const &container, std::function<bool(T const &)> const &predicate,
                                      std::size_t *const &position = nullptr) {
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
    [[nodiscard]] bool vectorContains(
            std::vector<T> const &container, T const &key, std::size_t *const &position = nullptr) {
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
    [[nodiscard]] bool vectorContains(
            std::vector<T *> const &container, T *&key, std::size_t *const &position = nullptr) {
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
    [[nodiscard]] bool vectorContains(
            std::vector<T *> const &container, T *const &key, std::size_t *const &position = nullptr) {
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
    [[nodiscard]] bool vectorContains(
            std::vector<T *> const &container, T const *&key, std::size_t *const &position = nullptr) {
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
    [[nodiscard]] bool vectorContains(std::vector<T *> const &container, T const *const &key,
                                      std::size_t *const &position = nullptr) {
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
    bool vectorRemoveFirstValueMatch(std::vector<T> &container, T const &value) {
        for (auto i = container.begin(); i != container.end(); ++i) {
            if (*i == value) {
                container.erase(i);
                return true;
            }
        }
        return false;
    }

    template<class T>
    bool vectorRemoveFirstValueMatch(
            std::vector<T> &container, std::function<bool(T const &)> const &predicate) {
        for (auto i = container.begin(); i != container.end(); ++i) {
            if (predicate(*i)) {
                container.erase(i);
                return true;
            }
        }
        return false;
    }

    template<class T>
    bool vectorRemoveFirstValueMatch(
            std::vector<T> &container, std::function<bool(T const &)> const &predicate, T &removedData) {
        for (auto i = container.begin(); i != container.end(); ++i) {
            if (predicate(*i)) {
                removedData = std::move(*i);
                container.erase(i);
                return true;
            }
        }
        return false;
    }

    template<class T>
    bool vectorRemoveFirstValueMatch(
            std::vector<T> &container, std::function<bool(T const &, int const &)> const &predicateWithIndex) {
        int index = 0;
        for (auto i = container.begin(); i != container.end(); ++i, ++index) {
            if (predicateWithIndex(*i, index)) {
                container.erase(i);
                return true;
            }
        }
        return false;
    }

    template<class T>
    bool vectorRemoveFirstValueMatch(
            std::vector<T> &container, std::function<bool(T const &, int const &)> const &predicateWithIndex,
            T &removedData) {
        int index = 0;
        for (auto i = container.begin(); i != container.end(); ++i, ++index) {
            if (predicateWithIndex(*i, index)) {
                removedData = std::move(*i);
                container.erase(i);
                return true;
            }
        }
        return false;
    }

    template<class T>
    void vectorRemoveAllValueMatches(std::vector<T> &container, T const &value) {
        for (auto i = container.begin(); i != container.end(); ++i) {
            if (*i == value) {
                container.erase(i);
                i--;
            }
        }
    }

    template<class T>
    void vectorRemoveAllValueMatches(std::vector<T> &container, std::function<bool(T const &)> const &predicate) {
        for (auto i = container.begin(); i != container.end(); ++i) {
            if (predicate(*i)) {
                container.erase(i);
                i--;
            }
        }
    }

    template<class T>
    void vectorRemoveAllValueMatches(std::vector<T> &container,
                                     std::function<bool(T const &, int const &)> const &predicateWithIndex) {
        int index = 0;
        for (auto i = container.begin(); i != container.end(); ++i, ++index) {
            if (predicateWithIndex(*i, index)) {
                container.erase(i);
                i--;
            }
        }
    }

    template<class T>
    void printVector(std::vector<T> const &x, std::function<bool(T const &)> const &elementFilter,
                     std::string const &separator = ", ", bool withNewline = true) {
        for (size_t i = 0; i < x.size(); i++) {
            if (elementFilter && elementFilter(x[i])) {
                continue;
            }
            if (i > 0) {
                std::cout << separator;
            }
            std::cout << x[i];
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T>
    void printVector(std::vector<T> const &x, std::string const &separator = ", ", bool withNewline = true) {
        printVector(x, std::function<bool(T const &)>{}, separator, withNewline);
    }

    template<class T>
    void printVector(std::vector<T> const &x, std::function<bool(T const &)> const &elementFilter,
                     std::function<std::string(T const &)> const &stringConversion, std::string const &separator = ", ",
                     bool withNewline = true) {
        for (size_t i = 0; i < x.size(); i++) {
            if (elementFilter && elementFilter(x[i])) {
                continue;
            }
            if (i > 0) {
                std::cout << separator;
            }
            std::cout << stringConversion(x[i]);
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T>
    void printVector(std::vector<T> const &x, std::function<std::string(T const &)> const &stringConversion,
                     std::string const &separator = ", ", bool withNewline = true) {
        printVector(x, std::function<bool(T const &)>{}, stringConversion, separator, withNewline);
    }

    template<class T>
    void printVector(std::vector<T> const &x, std::function<bool(T const &, size_t const &)> const &elementFilter,
                     std::function<std::string(T const &, size_t const &index)> const &stringConversion,
                     std::string const &separator = ", ", bool withNewline = true) {
        for (size_t i = 0; i < x.size(); i++) {
            if (elementFilter && elementFilter(x[i], i)) {
                continue;
            }
            if (i > 0) {
                std::cout << separator;
            }
            std::cout << stringConversion(x[i], i);
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T>
    void printVector(std::vector<T> const &x,
                     std::function<std::string(T const &, size_t const &index)> const &stringConversion,
                     std::string const &separator = ", ", bool withNewline = true) {
        printVector(x, std::function<bool(T const &, size_t const &)>{}, stringConversion, separator, withNewline);
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(
            std::vector<T> const &x, std::function<bool(T const &)> const &elementFilter,
            std::string const &separator = ", ") {
        std::stringstream s;
        for (size_t i = 0; i < x.size(); i++) {
            if (elementFilter && elementFilter(x[i])) {
                continue;
            }
            if (i > 0) {
                s << separator;
            }
            s << x[i];
        }
        return s.str();
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(std::vector<T> const &x, std::string const &separator = ", ") {
        return printVectorToString(x, std::function<bool(T const &)>{}, separator);
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(
            std::vector<T> const &x, std::function<bool(T const &)> const &elementFilter,
            std::function<std::string(T const &)> const &stringConversion, std::string const &separator = ", ") {
        std::stringstream s;
        for (size_t i = 0; i < x.size(); i++) {
            if (elementFilter && elementFilter(x[i])) {
                continue;
            }
            if (i > 0) {
                s << separator;
            }
            s << stringConversion(x[i]);
        }
        return s.str();
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(
            std::vector<T> const &x, std::function<std::string(T const &)> const &stringConversion,
            std::string const &separator = ", ") {
        return printVectorToString(x, std::function<bool(T const &)>{}, stringConversion, separator);
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(
            std::vector<T> const &x, std::function<bool(T const &, size_t const &)> const &elementFilter,
            std::function<std::string(T const &, size_t const &)> const &stringConversion,
            std::string const &separator = ", ") {
        std::stringstream s;
        for (size_t i = 0; i < x.size(); i++) {
            if (elementFilter && elementFilter(x[i], i)) {
                continue;
            }
            if (i > 0) {
                s << separator;
            }
            s << stringConversion(x[i], i);
        }
        return s.str();
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(
            std::vector<T> const &x, std::function<std::string(T const &, size_t const &)> const &stringConversion,
            std::string const &separator = ", ") {
        return printVectorToString(x, std::function<bool(T const &, size_t const &)>{}, stringConversion, separator);
    }

    template<class T>
    void printVector(T const *x, int size, std::function<bool(T const &)> const &elementFilter,
                     std::string const &separator = ", ", bool withNewline = true) {
        for (size_t i = 0; i < size; i++) {
            if (elementFilter && elementFilter(x[i])) {
                continue;
            }
            if (i > 0) {
                std::cout << separator;
            }
            std::cout << x[i];
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T>
    void printVector(T const *x, int size, std::string const &separator = ", ", bool withNewline = true) {
        printVector(x, size, std::function<bool(T const &)>{}, separator, withNewline);
    }

    template<class T>
    void printVector(T const *x, int size, std::function<bool(T const &)> const &elementFilter,
                     std::function<std::string(T const &)> const &stringConversion, std::string const &separator = ", ",
                     bool withNewline = true) {
        for (size_t i = 0; i < size; i++) {
            if (elementFilter && elementFilter(x[i])) {
                continue;
            }
            if (i > 0) {
                std::cout << separator;
            }
            std::cout << stringConversion(x[i]);
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T>
    void printVector(T const *x, int size, std::function<std::string(T const &)> const &stringConversion,
                     std::string const &separator = ", ", bool withNewline = true) {
        printVector(x, size, std::function<bool(T const &)>{}, stringConversion, separator, withNewline);
    }

    template<class T>
    void printVector(T const *x, int size, std::function<bool(T const &, size_t const &)> const &elementFilter,
                     std::function<std::string(T const &, size_t const &)> const &stringConversion,
                     std::string const &separator = ", ", bool withNewline = true) {
        for (size_t i = 0; i < size; i++) {
            if (elementFilter && elementFilter(x[i], i)) {
                continue;
            }
            if (i > 0) {
                std::cout << separator;
            }
            std::cout << stringConversion(x[i], i);
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T>
    void printVector(T const *x, int size,
                     std::function<std::string(T const &, size_t const &)> const &stringConversion,
                     std::string const &separator = ", ", bool withNewline = true) {
        printVectorToString(x, size, std::function<bool(T const &, size_t const &)>{}, stringConversion, separator,
                            withNewline);
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(
            T const *x, int size, std::function<bool(T const &)> const &elementFilter,
            std::string const &separator = ", ") {
        std::stringstream s;
        for (size_t i = 0; i < size; i++) {
            if (elementFilter && elementFilter(x[i])) {
                continue;
            }
            if (i > 0) {
                s << separator;
            }
            s << x[i];
        }
        return s.str();
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(T const *x, int size, std::string const &separator = ", ") {
        return printVectorToString(x, size, std::function<bool(T const &)>{}, separator);
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(
            T const *x, int size, std::function<bool(T const &)> const &elementFilter,
            std::function<std::string(T const &)> const &stringConversion, std::string const &separator = ", ") {
        std::stringstream s;
        for (size_t i = 0; i < size; i++) {
            if (elementFilter && elementFilter(x[i])) {
                continue;
            }
            if (i > 0) {
                s << separator;
            }
            s << stringConversion(x[i]);
        }
        return s.str();
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(
            T const *x, int size, std::function<std::string(T const &)> const &stringConversion,
            std::string const &separator = ", ") {
        return printVectorToString(x, size, std::function<bool(T const &)>{}, stringConversion, separator);
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(
            T const *x, int size, std::function<bool(T const &, size_t const &)> const &elementFilter,
            std::function<std::string(T const &, size_t const &)> const &stringConversion,
            std::string const &separator = ", ") {
        std::stringstream s;
        for (size_t i = 0; i < size; i++) {
            if (elementFilter && elementFilter(x[i], i)) {
                continue;
            }
            if (i > 0) {
                s << separator;
            }
            s << stringConversion(x[i], i);
        }
        return s.str();
    }

    template<class T>
    [[nodiscard]] std::string printVectorToString(
            T const *x, int size, std::function<std::string(T const &, size_t const &)> const &stringConversion,
            std::string const &separator = ", ") {
        return printVectorToString(x, size, std::function<bool(T const &, size_t const &)>{}, stringConversion,
                                   separator);
    }

    template<class T>
    [[nodiscard]] std::vector<T> mergeVectors(std::vector<T> const &v1, std::vector<T> const &v2) {
        std::vector<T> merged(v1);
        merged.insert(merged.end(), v2.begin(), v2.end());
        return merged;
    }

    template<typename T, typename Compare>
    [[nodiscard]] std::vector<std::size_t> getSortedIndicesOfVector(std::vector<T> const &v, Compare const &compare) {
        std::vector<std::size_t> indices(v.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(), [&](std::size_t i, std::size_t j) { return compare(v[i], v[j]); });
        return indices;
    }

    template<typename T>
    [[nodiscard]] std::vector<T> permuteVector(std::vector<T> const &v,
                                               std::vector<std::size_t> const &permutationIndices) {
        std::vector<T> permutation(v.size());
        std::transform(permutationIndices.begin(), permutationIndices.end(), permutation.begin(),
                       [&](std::size_t i) { return v[i]; });
        return permutation;
    }

    template<typename T>
    void permuteVectorInPlace(std::vector<T> &v, std::vector<std::size_t> const &permutationIndices) {
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
    void sortMultipleVectorsBasedOnPermutation(std::vector<std::size_t> const &permutationIndices, std::vector<T> &v) {
        permuteVectorInPlace(v, permutationIndices);
    }

    template<typename T1, typename... TArgs>
    void sortMultipleVectorsBasedOnPermutation(std::vector<std::size_t> const &permutationIndices, std::vector<T1> &v1,
                                               TArgs &... v2) {
        permuteVectorInPlace(v1, permutationIndices);
        sortMultipleVectorsBasedOnPermutation(permutationIndices, v2...);
    }

    template<typename T, typename Compare>
    void sortVectorsBasedOnFirst(std::vector<T> &v, Compare const &compare) {
        std::sort(v.begin(), v.end(), compare);
    }

    template<typename T1, typename Compare, typename... TArgs>
    void sortVectorsBasedOnFirst(std::vector<T1> &v1, Compare const &compare, TArgs &... v2) {
        auto permutationIndices = getSortedIndicesOfVector(v1, compare);
        sortMultipleVectorsBasedOnPermutation(permutationIndices, v1, v2...);
    }

    template<class T>
    [[nodiscard]] bool isSequenceStable(
            std::vector<T> const &sequence, T const &avg, std::function<double(T const &, T const &)> const &op,
            double stabilityThreshold = 0.5, StabilityCriterionOperation criterion = SUM, bool verbose = false) {
        double stabilityCoefficient = 0.;
        for (const auto &element: sequence) {
            switch (criterion) {
                case AVERAGE_OF_SUM:
                case SUM: {
                    stabilityCoefficient += op(element, avg);
                    break;
                }
                case AVERAGE_OF_SUM_OF_SQUARES: {
                    stabilityCoefficient += pow(op(element, avg), 2);
                    break;
                }
                default: {
                    throw std::runtime_error("Unknown StabilityCriterionOperation!");
                    break;
                }
            }
        }
        switch (criterion) {
            case AVERAGE_OF_SUM:
            case AVERAGE_OF_SUM_OF_SQUARES: {
                stabilityCoefficient /= sequence.size();
                break;
            }
            case SUM: {
                break;
            }
            default: {
                throw std::runtime_error("Unknown StabilityCriterionOperation!");
                break;
            }
        }
        if (verbose) {
            std::cout << "Stability coefficient = " << stabilityCoefficient << std::endl;
        }
        return stabilityCoefficient < stabilityThreshold;
    }

    template<class T>
    [[nodiscard]] bool isSequenceStable(std::vector<T> const &sequence, T const &avg, double stabilityThreshold = 0.5,
                                        StabilityCriterionOperation criterion = SUM, bool verbose = false) {
        return isSequenceStable(sequence, avg, [](T const &t1, T const &t2) { return std::abs(t1 - t2); },
                                stabilityThreshold, criterion, verbose);
    }

    template<class T>
    [[nodiscard]] bool isSequenceStable(std::vector<T> const &sequence, double stabilityThreshold = 0.5,
                                        StabilityCriterionOperation criterion = SUM, bool verbose = false) {
        if (sequence.empty()) {
            return true;
        }
        return isSequenceStable(sequence, average(sequence), stabilityThreshold, criterion, verbose);
    }

    template<class T>
    [[nodiscard]] bool isSequenceStable(
            std::vector<T> const &sequence, std::function<double(T const &, T const &)> const &op,
            double stabilityThreshold = 0.5, StabilityCriterionOperation criterion = SUM, bool verbose = false) {
        if (sequence.empty()) {
            return true;
        }
        return isSequenceStable(sequence, average(sequence), op, stabilityThreshold, criterion, verbose);
    }

    template<class T>
    [[nodiscard]] std::vector<T> reverseVector(std::vector<T> const &v) {
        auto res = v;
        std::reverse(res.begin(), res.end());
        return res;
    }

    template<class T>
    void reverseVectorInPlace(std::vector<T> &v) {
        std::reverse(v.begin(), v.end());
    }

    template<class T>
    [[nodiscard]] std::vector<T> vectorAppend(
            std::vector<T> const &container, std::vector<T> const &valuesToBeAppended) {
        auto res = container;
        res.insert(res.end(), valuesToBeAppended.begin(), valuesToBeAppended.end());
        return res;
    }

    template<class T>
    [[nodiscard]] std::vector<T> vectorAppend(
            std::vector<T> const &container, T const *valuesToBeAppended, size_t const &size) {
        std::vector<T> res = container;
        size_t containerSize = container.size();
        res.resize(containerSize + size);
        memcpy(res.data() + containerSize, valuesToBeAppended, size * sizeof(T));
        return res;
    }

    template<class T>
    void vectorAppendInPlace(std::vector<T> &container, std::vector<T> const &valuesToBeAppended) {
        container.insert(container.end(), valuesToBeAppended.begin(), valuesToBeAppended.end());
    }

    template<class T>
    void vectorAppendInPlace(std::vector<T> &container, T const *valuesToBeAppended, size_t const &size) {
        size_t containerSize = container.size();
        container.resize(containerSize + size);
        memcpy(container.data() + containerSize, valuesToBeAppended, size * sizeof(T));
    }

    template<class T>
    void vectorMoveAppendInPlace(std::vector<T> &container, std::vector<T> &&valuesToBeAppended) {
        if (container.empty()) {
            container = std::move(valuesToBeAppended);
        } else {
            container.reserve(container.size() + valuesToBeAppended.size());
            std::move(std::begin(valuesToBeAppended), std::end(valuesToBeAppended), std::back_inserter(container));
            valuesToBeAppended.clear();
        }
    }

    template<typename T>
    [[nodiscard]] bool vectorEquals(std::vector<T> const &v1, std::vector<T> const &v2) {
        if (v1.size() != v2.size()) {
            return false;
        }
        for (size_t i = 0; i < v1.size(); i++) {
            if (v1[i] != v2[i]) {
                return false;
            }
        }
        return true;
    }

    /**
     * @tparam T
     * @param v: the vector to splice
     * @param startIndex : the start index of the splice (inclusive)
     * @param endIndex : the end index of the splice (inclusive)
     * @return the vector from startIndex to endIndex inclusive
     */
    template<typename T>
    [[nodiscard]] std::vector<T> spliceVector(std::vector<T> const &v, int startIndex, int endIndex) {
        if (startIndex > endIndex) {
            return {};
        }
        startIndex = AndreiUtils::clamp<size_t>(startIndex, 0, v.size() - 1);
        endIndex = AndreiUtils::clamp<size_t>(endIndex, 0, v.size() - 1);
        return {v.begin() + startIndex, v.begin() + endIndex + 1};
    }

    template<typename T>
    [[nodiscard]] std::vector<T> removeDuplicates(std::vector<T> const &v) {
        std::set<T> s(v.begin(), v.end());
        std::vector<T> res(s.begin(), s.end());
        return res;
    }

    template<typename T>
    [[nodiscard]] T const &sampleFromVector(std::vector<T> const &v) {
        RandomNumberGenerator<int> sampler(0, v.size() - 1);
        return sampleFromVector(v, sampler);
    }

    template<typename T>
    [[nodiscard]] T &sampleFromVector(std::vector<T> &v) {
        RandomNumberGenerator<int> sampler(0, v.size() - 1);
        return sampleFromVector(v, sampler);
    }

    template<typename T>
    [[nodiscard]] T const &sampleFromVector(std::vector<T> const &v, RandomNumberGenerator<int> &sampler) {
        if (v.empty()) {
            throw std::runtime_error("Can not sample an element from an empty vector!");
        }
        int sampledIndex = sampler.sample();
        if (sampledIndex >= v.size()) {
            throw std::runtime_error("Sampled index is greater than or equal to the vector size!");
        }
        return v[sampledIndex];
    }

    template<typename T>
    [[nodiscard]] T &sampleFromVector(std::vector<T> &v, RandomNumberGenerator<int> &sampler) {
        if (v.empty()) {
            throw std::runtime_error("Can not sample an element from an empty vector!");
        }
        int sampledIndex = sampler.sample();
        if (sampledIndex >= v.size()) {
            throw std::runtime_error("Sampled index is greater than or equal to the vector size!");
        }
        return v[sampledIndex];
    }

    template<typename CastT, typename T>
    [[nodiscard]] std::vector<std::shared_ptr<CastT>> vectorCast(std::vector<std::shared_ptr<T>> const &v,
                                                                 bool withTypeCheck = true) {
        std::vector<std::shared_ptr<CastT>> res;
        if (v.empty()) {
            return res;
        }
        for (auto const &vElem: v) {
            auto vElemCast = std::dynamic_pointer_cast<CastT>(vElem);
            if (withTypeCheck && vElem != nullptr && vElemCast == nullptr) {
                throw std::runtime_error("Can not cast an element of the vector to the desired type!");
            } else {
                res.emplace_back(std::move(vElemCast));
            }
        }
        return res;
    }

    template<typename CastT, typename T>
    [[nodiscard]] std::vector<CastT *> vectorCast(std::vector<T *> const &v, bool withTypeCheck = true) {
        std::vector<CastT *> res;
        if (v.empty()) {
            return res;
        }
        for (auto const &vElem: v) {
            auto vElemCast = dynamic_cast<CastT *>(vElem);
            if (withTypeCheck && vElem != nullptr && vElemCast == nullptr) {
                throw std::runtime_error("Can not cast an element of the vector to the desired type!");
            } else {
                res.emplace_back(std::move(vElemCast));
            }
        }
        return res;
    }

    template<typename CastT, typename T>
    [[nodiscard]] std::vector<CastT> vectorCast(std::vector<T> const &v) {
        std::vector<CastT> res;
        if (v.empty()) {
            return res;
        }
        for (auto const &vElem: v) {
            res.emplace_back(vElem);
        }
        return res;
    }

    template<typename T>
    [[nodiscard]] T **new2dArray(long size1, long size2) {
        T **res = new T *[size1];
        for (long i = 0; i < size1; ++i) {
            res[i] = new T[size2];
        }
        return res;
    }

    template<typename T>
    void delete2dArray(T **mat, long size1) {
        for (long i = 0; i < size1; ++i) {
            delete[] mat[i];
        }
        delete[] mat;
    }

    template<typename OpResT, typename T>
    std::vector<OpResT>
    vectorOp(std::vector<T> const &v, std::function<void(std::vector<OpResT> &, T const &)> const &op) {
        std::vector<OpResT> res;
        for (auto const &elem: v) {
            op(res, elem);
        }
        return res;
    }

    template<typename OpResT, typename T>
    std::vector<OpResT>
    vectorOp(std::vector<T> &v, std::function<void(std::vector<OpResT> &, T &)> const &op) {
        std::vector<OpResT> res;
        for (auto &elem: v) {
            op(res, elem);
        }
        return res;
    }

    template<typename OpResT, typename T>
    std::vector<OpResT> vectorOp(std::vector<T> const &v, std::function<OpResT(T const &)> const &op) {
        std::vector<OpResT> res;
        for (auto const &elem: v) {
            res.emplace_back(std::move(op(elem)));
        }
        return res;
    }

    template<typename OpResT, typename T>
    std::vector<OpResT> vectorOp(std::vector<T> &v, std::function<OpResT(T &)> const &op) {
        std::vector<OpResT> res;
        for (auto &elem: v) {
            res.emplace_back(std::move(op(elem)));
        }
        return res;
    }
}
