//
// Created by Andrei on 22-Oct-21.
//

#ifndef ANDREIUTILS_UTILSOPENMP_HPP
#define ANDREIUTILS_UTILSOPENMP_HPP

#include <functional>
#include <omp.h>

namespace AndreiUtils {
    template<class T>
    void fastMemCopy(T *const dst, const T *const src, size_t size) {
        // memcpy(dst, src, sizeof(T) * size);
        #pragma omp parallel for shared(size)
        for (size_t i = 0; i < size; i++) {
            dst[i] = src[i];
        }
    }

    template<class T>
    void fastATimesSrcPlusB(T *const dst, const T *const src, size_t size, T a, T b) {
        #pragma omp parallel for shared(size)
        for (size_t i = 0; i < size; i++) {
            dst[i] = a * src[i] + b;
        }
    }

    template<class T>
    void fastSrcOp(T *const dst, const T *const src, size_t size, std::function<T(const T &)> op) {
        #pragma omp parallel for shared(size)
        for (size_t i = 0; i < size; i++) {
            dst[i] = op(src[i]);
        }
    }

    template<class Tin, class Tout>
    void fastSrcOp(Tout *const dst, const Tin *const src, size_t size, std::function<Tout(const Tin &)> op) {
        #pragma omp parallel for shared(size)
        for (size_t i = 0; i < size; i++) {
            dst[i] = op(src[i]);
        }
    }

    template<class T>
    void fastForLoop(T *const array, size_t size, std::function<void(T *const, size_t, size_t)> op,
                     size_t increment = 1) {
        #pragma omp parallel for shared(size, increment)
        for (size_t i = 0; i < size; i += increment) {
            op(array, i, increment);
        }
    }
}

#endif //ANDREIUTILS_UTILSOPENMP_HPP
