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
// Created by Andrei on 21.10.22.
//

#ifndef ANDREIUTILS_INTERPOLATOR_HPP
#define ANDREIUTILS_INTERPOLATOR_HPP

#include <vector>

namespace AndreiUtils {
    template<class T>
    class Interpolator {
    public:
        Interpolator() = default;

        virtual ~Interpolator() { this->clear(); }

        virtual void clear() { this->result.clear(); }

        [[nodiscard]] std::vector<T> &getResult() { return this->result; }

        [[nodiscard]] std::vector<T> const &getResult() const { return this->result; }

    protected:
        std::vector<T> result;
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_INTERPOLATOR_HPP
