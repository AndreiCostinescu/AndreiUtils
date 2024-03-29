//
// Created by Andrei on 13.11.20.
//

#ifndef ANDREIUTILS_UTILS_HPP
#define ANDREIUTILS_UTILS_HPP

#include <cmath>
#include <type_traits>
#include <stdexcept>
#include <utility>

namespace AndreiUtils {
    template<typename T>
    void swapData(T &a, T &b) {
        T t(std::move(a));
        a = std::move(b);
        b = std::move(t);
    }

    template<typename T>
    void swapPtrData(T *a, T *b) {
        if (a == nullptr || b == nullptr) {
            throw std::runtime_error("Can not swap null pointer data");
        }
        swapData(*a, *b);
    }

    template<typename T>
    inline int intRound(const T a) {
        return lround(a);
    }

    template<typename T>
    inline bool less(const T a, const T b, double tol = 1e-9) {
        if (std::is_integral<T>::value) {
            return a < b;
        } else {
            return (b - a) > tol;
        }
    }

    template<typename T>
    inline bool lessEqual(const T a, const T b, double tol = 1e-9) {
        if (std::is_integral<T>::value) {
            return a <= b;
        } else {
            return (b - a) > -tol;
        }
    }

    template<typename T>
    inline bool greater(const T a, const T b, double tol = 1e-9) {
        if (std::is_integral<T>::value) {
            return a > b;
        } else {
            return (a - b) > tol;
        }
    }

    template<typename T>
    inline bool greaterEqual(const T a, const T b, double tol = 1e-9) {
        if (std::is_integral<T>::value) {
            return a >= b;
        } else {
            return (a - b) > -tol;
        }
    }

    template<typename T>
    inline T fastAbs(const T x) {
        return (less<T>(x, 0) ? -x : x);
    }

    template<typename T>
    inline bool equal(const T a, const T b, double tol = 1e-9) {
        if (std::is_integral<T>::value) {
            return a == b;
        } else {
            return (fastAbs(a - b) < tol);
        }
    }

    template<typename T>
    inline T fastMin(const T a, const T b) {
        return (less<T>(a, b) ? a : b);
    }

    template<typename T>
    inline T fastMax(const T a, const T b) {
        return (greater<T>(a, b) ? a : b);
    }

    template<typename T>
    inline T clamp(T const &v, T const &minV, T const &maxV) {
        return fastMin(fastMax(v, minV), maxV);
    }

    template<typename T>
    inline T average(T const &a, T const &b) {
        return a + (b - a) / 2;
    }
}

#endif //ANDREIUTILS_UTILS_HPP
