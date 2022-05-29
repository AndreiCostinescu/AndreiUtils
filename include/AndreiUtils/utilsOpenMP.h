//
// Created by Andrei on 29.05.22.
//

#ifndef ANDREIUTILS_UTILSOPENMP_H
#define ANDREIUTILS_UTILSOPENMP_H

#include <omp.h>

namespace AndreiUtils {
    int getOMPActiveThreadNumber();

    int getNumberOfActiveOMPThreads();

    void setNumberOfOMPThreads(int nrThread);

    int maxNumberOfOMPThreads();
}

#endif //ANDREIUTILS_UTILSOPENMP_H
