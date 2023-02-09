//
// Created by Andrei on 13.11.20.
//

#ifndef ANDREIUTILS_UTILS_HPP
#define ANDREIUTILS_UTILS_HPP

#include <AndreiUtils/traits/InstanceOf.hpp>
#include <cmath>
#include <type_traits>
#include <stdexcept>
#include <utility>

namespace AndreiUtils {
    template<typename TypeCheck, typename InstanceType>
    bool instanceOf(InstanceType const &val) {
        return InstanceOf<TypeCheck, InstanceType,
                std::is_polymorphic<typename std::remove_pointer<InstanceType>::type>::value>::get(val);
    }

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

    template<typename T>
    bool inRangeLELE(T const &value, T const &min, T const &max, double const &tol = 1e-9) {
        return lessEqual(min, value, tol) && lessEqual(value, max, tol);
    }

    template<typename T>
    bool inRangeLLE(T const &value, T const &min, T const &max, double const &tol = 1e-9) {
        return less(min, value, tol) && lessEqual(value, max, tol);
    }

    template<typename T>
    bool inRangeLEL(T const &value, T const &min, T const &max, double const &tol = 1e-9) {
        return lessEqual(min, value, tol) && less(value, max, tol);
    }

    template<typename T>
    bool inRangeLL(T const &value, T const &min, T const &max, double const &tol = 1e-9) {
        return less(min, value, tol) && less(value, max, tol);
    }

    template<typename T>
    bool inRange(T const &value, T const &min, T const &max, double const &tol = 1e-9) {
        return inRangeLELE(value, min, max, tol);
    }

    template<typename T>
    void bringValueInCircularInterval(T &v, T const &minInterval, T const &maxInterval, double tol = 1e-9) {
        T intervalSize = maxInterval - minInterval;
        while (AndreiUtils::less<T>(v, minInterval, tol)) {
            v += intervalSize;
        }
        while (AndreiUtils::less<T>(maxInterval, v, tol)) {
            v -= intervalSize;
        }
    }

    template<typename T>
    T getValueInCircularInterval(T const &v, T const &minInterval, T const &maxInterval, double tol = 1e-9) {
        T vCopy = v;
        bringValueInCircularInterval(vCopy, minInterval, maxInterval, tol);
        return vCopy;
    }
}

#endif //ANDREIUTILS_UTILS_HPP
