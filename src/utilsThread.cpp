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
// Created by Andrei on 27.08.21.
//

#include <AndreiUtils/utilsThread.h>
#include <chrono>
#include <thread>

using namespace std;

int const AndreiUtils::threadSleepTime = 1;

void AndreiUtils::sleepMSec(int mSec) {
    chrono::milliseconds milliseconds(mSec);
    this_thread::sleep_for(milliseconds);
}

void AndreiUtils::sleepUSec(int uSec) {
    chrono::microseconds microseconds(uSec);
    this_thread::sleep_for(microseconds);
}

void AndreiUtils::threadBarrier(volatile atomic<int> &threadVariable, bool yieldOrSleep, bool waitUntilZero) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
    while (waitUntilZero == atomic_load(&threadVariable)) {
        if (yieldOrSleep) {
            threadYield();
        } else {
            sleepMSec(threadSleepTime);
        }
    }
#pragma clang diagnostic pop
}

void AndreiUtils::threadBarrier(volatile atomic<bool> &threadVariable, bool yieldOrSleep, bool waitUntilFalse) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
    while (waitUntilFalse == threadVariable.load()) {
        if (yieldOrSleep) {
            threadYield();
        } else {
            sleepMSec(threadSleepTime);
        }
    }
#pragma clang diagnostic pop
}

void AndreiUtils::threadYield() { this_thread::yield(); }
