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
// Created by Andrei on 28.05.22.
//

#ifndef ANDREIUTILS_UTILSPOSE_HPP
#define ANDREIUTILS_UTILSPOSE_HPP

#include <AndreiUtils/classes/DualQuaternion.hpp>

namespace AndreiUtils {
    template<class T>
    DualQuaternion<T> getRelativePoseCommonStart(const DualQuaternion<T> &q_c_1, const DualQuaternion<T> &q_c_2) {
        // We want transformation from 1 to 2: q_1_2 = q_2 * q_1.inverse()
        return q_c_2 * q_c_1.dualQuaternionInverse();
    }

    template<class T>
    DualQuaternion<T> getRelativePoseCommonEnd(const DualQuaternion<T> &q_1_c, const DualQuaternion<T> &q_2_c) {
        // We want transformation from 1 to 2: q_1_2 = q_2.inverse() * q_1
        return q_2_c.dualQuaternionInverse() * q_1_c;
    }
} // namespace AndreiUtils

#endif // ANDREIUTILS_UTILSPOSE_HPP
