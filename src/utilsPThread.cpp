// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 17.06.22.
//

#include <AndreiUtils/utilsPThread.h>
#include <cstring>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

void AndreiUtils::setThreadPriority(int priority) { setThreadPriority(priority, pthread_self()); }

void AndreiUtils::setThreadPriority(int priority, thread &t) { setThreadPriority(priority, t.native_handle()); }

void AndreiUtils::setThreadPriority(int priority, pthread_t t) {
    sched_param sch{};
    int policy;
    pthread_getschedparam(t, &policy, &sch);
    sch.sched_priority = priority;
    if (pthread_setschedparam(t, SCHED_FIFO, &sch)) {
        cout << "setschedparam failed: " << strerror(errno) << '\n';
    }
}

int AndreiUtils::getThreadPriority() { return getThreadPriority(pthread_self()); }

int AndreiUtils::getThreadPriority(thread &t) { return getThreadPriority(t.native_handle()); }

int AndreiUtils::getThreadPriority(pthread_t t) {
    sched_param sch{};
    int policy;
    pthread_getschedparam(t, &policy, &sch);
    return sch.sched_priority;
}
