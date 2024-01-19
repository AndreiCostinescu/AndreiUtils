//
// Created by Andrei on 13.11.20.
//

#ifndef ANDREIUTILS_UTILS_HPP
#define ANDREIUTILS_UTILS_HPP

#include <AndreiUtils/traits/InstanceOf.hpp>
#include <cmath>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <utility>

namespace AndreiUtils {
    template<typename TypeCheck, typename InstanceType>
    bool instanceOf(InstanceType const &val) {
        return InstanceOf<TypeCheck, InstanceType,
                std::is_polymorphic<typename std::remove_pointer<InstanceType>::type>::value>::get(val);
    }

    template<typename TypeCheck, typename InstanceType>
    bool pointerInstanceOf(std::shared_ptr<InstanceType> const &val) {
        return InstanceOf<TypeCheck, InstanceType *,
                std::is_polymorphic<typename std::remove_pointer<InstanceType>::type>::value>::get(val.get());
    }

    template<typename TypeCheck, typename InstanceType>
    bool pointerInstanceOf(std::unique_ptr<InstanceType> const &val) {
        return InstanceOf<TypeCheck, InstanceType *,
                std::is_polymorphic<typename std::remove_pointer<InstanceType>::type>::value>::get(val.get());
    }

    template<typename TypeCheck, typename InstanceType>
    bool pointerInstanceOf(std::weak_ptr<InstanceType> const &val) {
        return InstanceOf<TypeCheck, InstanceType *,
                std::is_polymorphic<typename std::remove_pointer<InstanceType>::type>::value>::get(val.get());
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
    inline int intRound(T const &a) {
        return lround(a);
    }

    template<typename T>
    inline bool less(T const &a, T const &b, double tol = 1e-9) {
        if (std::is_integral<T>::value) {
            return a < b;
        } else {
            return (b - a) > tol;
        }
    }

    template<typename T>
    inline bool lessEqual(T const &a, T const &b, double tol = 1e-9) {
        if (std::is_integral<T>::value) {
            return a <= b;
        } else {
            return (b - a) > -tol;
        }
    }

    template<typename T>
    inline bool greater(T const &a, T const &b, double tol = 1e-9) {
        if (std::is_integral<T>::value) {
            return a > b;
        } else {
            return (a - b) > tol;
        }
    }

    template<typename T>
    inline bool greaterEqual(T const &a, T const &b, double tol = 1e-9) {
        if (std::is_integral<T>::value) {
            return a >= b;
        } else {
            return (a - b) > -tol;
        }
    }

    template<typename T>
    inline T fastAbs(T const &x) {
        return (less<T>(x, 0) ? -x : x);
    }

    template<typename T>
    inline bool equal(T const &a, T const &b, double tol = 1e-9) {
        if (std::is_integral<T>::value) {
            return a == b;
        } else {
            return (fastAbs(a - b) < tol);
        }
    }

    template<typename T>
    inline T fastMin(T const &a, T const &b) {
        return (less<T>(a, b) ? a : b);
    }

    template<typename T>
    inline T fastMax(T const &a, T const &b) {
        return (greater<T>(a, b) ? a : b);
    }

    template<typename T>
    [[nodiscard]] inline T clamp(T const &v, T const &minV, T const &maxV) {
        return fastMin(fastMax(v, minV), maxV);
    }

    template<typename T>
    [[nodiscard]] inline T clampLow(T const &v, T const &minV) {
        return fastMax(v, minV);
    }

    template<typename T>
    [[nodiscard]] inline T clampHigh(T const &v, T const &maxV) {
        return fastMin(v, maxV);
    }

    template<typename T>
    void clampInPlace(T &v, T const &minV, T const &maxV) {
        v = clamp(v, minV, maxV);
    }

    template<typename T>
    void clampLowInPlace(T &v, T const &minV) {
        v = clampLow(v, minV);
    }

    template<typename T>
    void clampHighInPlace(T &v, T const &maxV) {
        v = clampHigh(v, maxV);
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

    template<typename T>
    T signPosNeg(bool positive) {
        return positive ? T(1) : T(-1);
    }

    template<typename T>
    int signOf(T val) {
        return (T(0) < val) - (val < T(0));
    }
}

#endif //ANDREIUTILS_UTILS_HPP
