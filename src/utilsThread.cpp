//
// Created by andrei on 27.08.21.
//

#include <AndreiUtils/utilsThread.h>
#include <chrono>
#include <thread>

using namespace std;

const int AndreiUtils::threadSleepTime = 1;

void AndreiUtils::sleepMSec(int mSec) {
    chrono::milliseconds milliseconds(mSec);
    this_thread::sleep_for(milliseconds);
}

void AndreiUtils::sleepUSec(int uSec) {
    chrono::microseconds microseconds(uSec);
    this_thread::sleep_for(microseconds);
}

void AndreiUtils::threadBarrier(volatile atomic<int> &threadVariable, bool yieldOrSleep) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
    while (atomic_load(&threadVariable)) {
        if (yieldOrSleep) {
            this_thread::yield();
        } else {
            sleepMSec(threadSleepTime);
        }
    }
#pragma clang diagnostic pop
}
