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
// Created by Andrei on 04.12.2022.
//

#include <AndreiUtils/utilsOpenMP.hpp>
#include <AndreiUtils/utilsVector.hpp>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

void testOMPUtils() {
    cout << "Default settings:" << endl;
    printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), maxNumberOfOMPThreads());
#pragma omp parallel default(none)
    {
#pragma omp single
        printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    }
#pragma omp parallel default(none) num_threads(3)
    {
#pragma omp single
        printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    }
    cout << endl;

    cout << "After setting #omp threads to 12:" << endl;
    setNumberOfOMPThreads(12);
    printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), maxNumberOfOMPThreads());
#pragma omp parallel default(none)
    {
#pragma omp single
        printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    }
#pragma omp parallel default(none) num_threads(3)
    {
#pragma omp single
        printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    }
}

void testFastForLoop() {
    vector<int> v(4 * maxNumberOfOMPThreads());
    printVector(v);
    fastForLoop<int>(v, [](int threadID, vector<int> &_v, size_t index, size_t) { _v[index] = threadID; });
    printVector(v);
    setNumberOfOMPThreads(16);
    fastForLoop<int>(v, [](int threadID, vector<int> &_v, size_t index, size_t) { _v[index] = threadID; });
    printVector(v);
}

void testFastMemCopy() {
    int n = 10;
    auto *dstData = new uint8_t[n], *srcData = new uint8_t[n];
    for (int i = 0; i < n; i++) {
        srcData[i] = i;
    }

    fastMemCopy(dstData, srcData, n);

    printVector(dstData, n);

    delete[] dstData;
    delete[] srcData;
}

void testFastATimesSrcPlusB() {
    int n = 10;
    auto *dstData = new uint8_t[n], *srcData = new uint8_t[n];
    for (int i = 0; i < n; i++) {
        srcData[i] = i;
    }

    fastATimesSrcPlusB<uint8_t>(dstData, srcData, n, 20, 5);

    printVector(dstData, n);

    delete[] dstData;
    delete[] srcData;
}

int main() {
    cout << "Hello World!" << endl;

    // testOMPUtils();
    // testFastForLoop();
    testFastMemCopy();
    testFastATimesSrcPlusB();

    return 0;
}
