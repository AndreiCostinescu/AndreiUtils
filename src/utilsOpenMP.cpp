/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei on 29.05.22.
//

#include <AndreiUtils/utilsOpenMP.h>

using namespace AndreiUtils;

int AndreiUtils::getOMPActiveThreadNumber() { return omp_get_thread_num(); }

int AndreiUtils::getNumberOfActiveOMPThreads() { return omp_get_num_threads(); }

void AndreiUtils::setNumberOfOMPThreads(int nrThreads) { return omp_set_num_threads(nrThreads); }

int AndreiUtils::maxNumberOfOMPThreads() {
    /*
    int n = 0;
    #pragma omp parallel reduction(+:n) default(none)
    n += 1;
    return n;
    //*/
    return omp_get_max_threads();
}
