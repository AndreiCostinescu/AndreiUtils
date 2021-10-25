//
// Created by Andrei on 22-Oct-21.
//

#ifndef ANDREIUTILS_UTILSOPENMP_HPP
#define ANDREIUTILS_UTILSOPENMP_HPP

#include <omp.h>

namespace AndreiUtils {
    template<class T>
    void fastMemCopy(T *const dst, T *const src, size_t size) {
        // memcpy(dst, src, sizeof(T) * size);
        #pragma omp parallel for shared(size) default(none)
        for (size_t i = 0; i < size; i++) {
            dst[i] = src[i];
        }
    }
}

#endif //ANDREIUTILS_UTILSOPENMP_HPP
