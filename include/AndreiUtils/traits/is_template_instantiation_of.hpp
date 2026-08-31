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
// Created by Andrei on 14.05.26.
//

#pragma once

#include <type_traits>

namespace AndreiUtils {
    // check whether T is an instantiation of template C
    template<typename, template<class...> class>
    struct is_template_instantiation_of : std::false_type {};

    template<template<class...> class C, class... Args>
    struct is_template_instantiation_of<C<Args...>, C> : std::true_type {};

    template<typename T, template<class...> class C>
    inline constexpr bool is_template_instantiation_of_v = is_template_instantiation_of<T, C>::value;
} // namespace AndreiUtils
