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
// Created by Andrei on 13.09.2021.
//

#include <AndreiUtils/classes/Timer.hpp>
#include <stdexcept>

using namespace AndreiUtils;
using namespace std;

Timer::Timer(bool startTiming) {
    if (startTiming) {
        this->start();
    }
}

void Timer::start() { this->startTime = clockType::now(); }

void Timer::reset() { this->start(); }

double Timer::measure(string const &timeUnit) const { return this->measure(convertStringToTimeUnit(timeUnit)); }

double Timer::measure(TimeUnit timeUnit) const {
    return chrono::duration<double>(clockType::now() - this->startTime).count() *
           getMultiplicationFactorRelativeToSeconds(timeUnit);
}
