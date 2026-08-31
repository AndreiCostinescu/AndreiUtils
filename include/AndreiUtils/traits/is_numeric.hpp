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
// Created by Andrei on 20.11.23.
//

#pragma once

#include <type_traits>

namespace AndreiUtils {
    template<typename T, class Enable = void>
    struct is_numeric {
        static const bool value = false;
    };

    template<class T>
    struct is_numeric<T, typename std::enable_if<std::is_integral<T>::value>::type> {
        static const bool value = true;
    };

    template<class T>
    struct is_numeric<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
        static const bool value = true;
    };

#if __cplusplus > 201402L
    template<typename T>
    inline constexpr bool is_numeric_v = is_numeric<T>::value;
#endif
} // namespace AndreiUtils
