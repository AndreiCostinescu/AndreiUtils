//
// Created by andrei on 27.08.21.
//

#ifndef ANDREIUTILS_UTILSTHREAD_H
#define ANDREIUTILS_UTILSTHREAD_H

#include <atomic>

namespace AndreiUtils {
    extern const int threadSleepTime;

    void sleepMSec(int mSec);

    void sleepUSec(int uSec);

    void threadBarrier(volatile std::atomic<int> &threadVariable, bool yieldOrSleep = false);
}

#endif //ANDREIUTILS_UTILSTHREAD_H
