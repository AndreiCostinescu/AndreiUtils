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
// Created by Andrei Costinescu (andrei.costinescu@tum.de) on 14.03.2021.
//

#ifndef ANDREIUTILS_CLASSES_FORKSHAREDVARIABLE_HPP
#define ANDREIUTILS_CLASSES_FORKSHAREDVARIABLE_HPP

#if defined(_WIN32) || defined(_WIN64)

#include <mman.h>

#else

#include <sys/mman.h>

#endif

namespace AndreiUtils {
    template<class T>
    class ForkSharedVariable {
    public:
        ForkSharedVariable() {
            this->variable = (T *) mmap(nullptr, sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        }

        explicit ForkSharedVariable(T initialValue) : ForkSharedVariable() { *(this->variable) = initialValue; }

        ~ForkSharedVariable() { munmap(this->variable, sizeof(T)); }

        T get() { return *(this->variable); }

        void set(T value) { *(this->variable) = value; }

    private:
        T *variable;
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_CLASSES_FORKSHAREDVARIABLE_HPP
