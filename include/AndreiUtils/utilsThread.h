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
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSTHREAD_H
#define ANDREIUTILS_UTILSTHREAD_H

#include <atomic>

namespace AndreiUtils {
    extern int const threadSleepTime;

    void sleepMSec(int mSec);

    void sleepUSec(int uSec);

    void threadBarrier(volatile std::atomic<int> &threadVariable, bool yieldOrSleep = false, bool waitUntilZero = true);

    void threadBarrier(volatile std::atomic<bool> &threadVariable, bool yieldOrSleep = false,
                       bool waitUtilFalse = true);

    void threadYield();
} // namespace AndreiUtils

#endif // ANDREIUTILS_UTILSTHREAD_H
