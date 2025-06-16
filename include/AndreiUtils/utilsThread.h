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
}

#endif //ANDREIUTILS_UTILSTHREAD_H
