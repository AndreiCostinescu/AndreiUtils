//
// Created by Andrei on 13.11.20.
//

#ifndef ANDREIUTILS_UTILS_HPP
#define ANDREIUTILS_UTILS_HPP

#include <cmath>
#include <stdexcept>
#include <utility>

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
}

#endif //ANDREIUTILS_UTILS_HPP
