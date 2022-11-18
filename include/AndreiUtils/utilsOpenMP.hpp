//
// Created by Andrei on 22-Oct-21.
//

#ifndef ANDREIUTILS_UTILSOPENMP_HPP
#define ANDREIUTILS_UTILSOPENMP_HPP

#include <AndreiUtils/utilsOpenMP.h>
#include <functional>
#include <vector>

namespace AndreiUtils {
    template<class T>
    void fastMemCopy(T *const dst, T const *const src, size_t size) {
        // memcpy(dst, src, sizeof(T) * size);
        // dst and src are predetermined shared in gcc 7?!
        #ifdef __GNUG__
        #if __GNUG__ > 7
        #pragma omp parallel for shared(dst, src, size) default(none)
        #else
        #pragma omp parallel for shared(size) default(none)
        #endif
        #else
        #pragma omp parallel for shared(size) default(none)
        #endif
        for (size_t i = 0; i < size; i++) {
            dst[i] = src[i];
        }
    }

    template<class T>
    T *fastCreateCopy(T const *const src, size_t size) {
        auto *dst = new T[size];
        fastMemCopy(dst, src, size);
        return dst;
    }

    template<class T>
    void fastATimesSrcPlusB(T *const dst, T const *const src, size_t size, T a, T b) {
        // dst and src are predetermined shared?!
        #ifdef __GNUG__
        #if __GNUG__ > 7
        #pragma omp parallel for shared(dst, src, size, a, b) default(none)
        #else
        #pragma omp parallel for shared(size, a, b) default(none)
        #endif
        #else
        #pragma omp parallel for shared(size, a, b) default(none)
        #endif
        for (size_t i = 0; i < size; i++) {
            dst[i] = a * src[i] + b;
        }
    }

    template<class T>
    void fastSrcOp(T *const dst, T const *const src, size_t size, std::function<T(T const &)> op) {
        // dst and src are predetermined shared?!
        #ifdef __GNUG__
        #if __GNUG__ > 7
        #pragma omp parallel for shared(dst, src, size, op) default(none)
        #else
        #pragma omp parallel for shared(size, op) default(none)
        #endif
        #else
        #pragma omp parallel for shared(size, op) default(none)
        #endif
        for (size_t i = 0; i < size; i++) {
            dst[i] = op(src[i]);
        }
    }

    template<class Tin, class Tout>
    void fastSrcOp(Tout *const dst, Tin const *const src, size_t size, std::function<Tout(Tin const &)> op) {
        // dst and src are predetermined shared?!
        #ifdef __GNUG__
        #if __GNUG__ > 7
        #pragma omp parallel for shared(dst, src, size, op) default(none)
        #else
        #pragma omp parallel for shared(size, op) default(none)
        #endif
        #else
        #pragma omp parallel for shared(size, op) default(none)
        #endif
        for (size_t i = 0; i < size; i++) {
            dst[i] = op(src[i]);
        }
    }

    template<class T>
    void fastForLoop(T *const array, size_t size, std::function<void(T *const, size_t, size_t)> op,
                     size_t increment = 1) {
        // array is predetermined shared?!
        #ifdef __GNUG__
        #if __GNUG__ > 7
        #pragma omp parallel for shared(array, size, increment, op) default(none)
        #else
        #pragma omp parallel for shared(size, increment, op) default(none)
        #endif
        #else
        #pragma omp parallel for shared(size, increment, op) default(none)
        #endif
        for (size_t i = 0; i < size; i += increment) {
            op(array, i, increment);
        }
    }

    template<class T>
    void fastForLoop(T *const array, size_t size, std::function<void(T *const, size_t, size_t, size_t)> op,
                     size_t increment = 1) {
        // array is predefined shared?!
        #ifdef __GNUG__
        #if __GNUG__ > 7
        #pragma omp parallel for shared(array, size, increment, op) default(none)
        #else
        #pragma omp parallel for shared(size, increment, op) default(none)
        #endif
        #else
        #pragma omp parallel for shared(size, increment, op) default(none)
        #endif
        for (size_t i = 0; i < size; i += increment) {
            op(array, size, i, increment);
        }
    }

    template<class T>
    void fastForLoop(T *const array, size_t size, std::function<void(int, T *const, size_t, size_t)> op,
                     size_t increment = 1) {
        // array is predefined shared?!
        #ifdef __GNUG__
        #if __GNUG__ > 7
        #pragma omp parallel for shared(array, size, increment, op) default(none)
        #else
        #pragma omp parallel for shared(size, increment, op) default(none)
        #endif
        #else
        #pragma omp parallel for shared(size, increment, op) default(none)
        #endif
        for (size_t i = 0; i < size; i += increment) {
            op(getOMPActiveThreadNumber(), array, i, increment);
        }
    }

    template<class T>
    void fastForLoop(T *const array, size_t size, std::function<void(int, T *const, size_t, size_t, size_t)> op,
                     size_t increment = 1) {
        // array is predefined shared?!
        #ifdef __GNUG__
        #if __GNUG__ > 7
        #pragma omp parallel for shared(array, size, increment, op) default(none)
        #else
        #pragma omp parallel for shared(size, increment, op) default(none)
        #endif
        #else
        #pragma omp parallel for shared(size, increment, op) default(none)
        #endif
        for (size_t i = 0; i < size; i += increment) {
            op(getOMPActiveThreadNumber(), array, size, i, increment);
        }
    }

    template<class T>
    void fastForLoop(std::vector<T> &array, std::function<void(std::vector<T> &, size_t, size_t)> op,
                     size_t increment = 1) {
        size_t size = array.size();
        #pragma omp parallel for shared(array, size, increment, op) default(none)
        for (size_t i = 0; i < size; i += increment) {
            op(array, i, increment);
        }
    }

    template<class T>
    void fastForLoop(std::vector<T> const &array, std::function<void(std::vector<T> const &, size_t, size_t)> op,
                     size_t increment = 1) {
        size_t size = array.size();
        #pragma omp parallel for shared(array, size, increment, op) default(none)
        for (size_t i = 0; i < size; i += increment) {
            op(array, i, increment);
        }
    }

    template<class T>
    void fastForLoop(std::vector<T> &array, std::function<void(int, std::vector<T> &, size_t, size_t)> op,
                     size_t increment = 1) {
        size_t size = array.size();
        #pragma omp parallel for shared(array, size, increment, op) default(none)
        for (size_t i = 0; i < size; i += increment) {
            op(getOMPActiveThreadNumber(), array, i, increment);
        }
    }

    template<class T>
    void fastForLoop(std::vector<T> const &array, std::function<void(int, std::vector<T> const &, size_t, size_t)> op,
                     size_t increment = 1) {
        size_t size = array.size();
        #pragma omp parallel for shared(array, size, increment, op) default(none)
        for (size_t i = 0; i < size; i += increment) {
            op(getOMPActiveThreadNumber(), array, i, increment);
        }
    }
}

#endif //ANDREIUTILS_UTILSOPENMP_HPP
