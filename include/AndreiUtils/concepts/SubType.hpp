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
// Created by Andrei on 19.12.25.
//

#pragma once

#include <concepts>

namespace AndreiUtils {
    // the first condition is to handle non-class types such as int, float, double
    // (which fail the std::is_base_of<int, int> condition...)
    template<typename T, typename SubT>
    concept SubType = std::is_same_v<std::remove_const_t<T>, std::remove_const_t<SubT>> ||
                      std::is_base_of_v<std::remove_const_t<T>, std::remove_const_t<SubT>>;
} // namespace AndreiUtils
