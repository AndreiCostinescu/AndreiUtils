//
// Created by Andrei on 29.05.22.
//

#include <AndreiUtils/utilsOpenMP.h>

using namespace AndreiUtils;

int AndreiUtils::getOMPActiveThreadNumber() {
    return omp_get_thread_num();
}

int AndreiUtils::getNumberOfActiveOMPThreads() {
    return omp_get_num_threads();
}

void AndreiUtils::setNumberOfOMPThreads(int nrThreads) {
    return omp_set_num_threads(nrThreads);
}

int AndreiUtils::maxNumberOfOMPThreads() {
    /*
    int n = 0;
    #pragma omp parallel reduction(+:n) default(none)
    n += 1;
    return n;
    //*/
    return omp_get_max_threads();
}
