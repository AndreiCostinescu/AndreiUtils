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
}

#endif //ANDREIUTILS_UTILSPTHREAD_H
