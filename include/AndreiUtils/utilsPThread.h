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
// Created by Andrei on 17.06.22.
//

#ifndef ANDREIUTILS_UTILSPTHREAD_H
#define ANDREIUTILS_UTILSPTHREAD_H

#include <pthread.h>
#include <thread>

namespace AndreiUtils {
    void setThreadPriority(int priority);

    void setThreadPriority(int priority, std::thread &t);

    void setThreadPriority(int priority, pthread_t t);

    int getThreadPriority();

    int getThreadPriority(std::thread &t);

    int getThreadPriority(pthread_t t);
} // namespace AndreiUtils

#endif // ANDREIUTILS_UTILSPTHREAD_H
