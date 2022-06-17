//
// Created by root on 17.06.22.
//

#include "AndreiUtils/utilsPThread.h"
#include <cstring>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

void AndreiUtils::setThreadPriority(int priority) {
    setThreadPriority(priority, pthread_self());
}

void AndreiUtils::setThreadPriority(int priority, thread &t) {
    setThreadPriority(priority, t.native_handle());
}

void AndreiUtils::setThreadPriority(int priority, pthread_t t) {
    sched_param sch{};
    int policy;
    pthread_getschedparam(t, &policy, &sch);
    sch.sched_priority = priority;
    if (pthread_setschedparam(t, SCHED_FIFO, &sch)) {
        cout << "setschedparam failed: " << strerror(errno) << '\n';
    }
}

int AndreiUtils::getThreadPriority() {
    return getThreadPriority(pthread_self());
}

int AndreiUtils::getThreadPriority(thread &t) {
    return getThreadPriority(t.native_handle());
}

int AndreiUtils::getThreadPriority(pthread_t t) {
    sched_param sch{};
    int policy;
    pthread_getschedparam(t, &policy, &sch);
    return sch.sched_priority;
}
