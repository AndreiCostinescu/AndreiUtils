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
// Created by Andrei on 01.07.24.
//

#pragma once

#include <type_traits>

namespace AndreiUtils {
    template<class...>
    using void_t = void;

    struct nonesuch {
        nonesuch() = delete;

        ~nonesuch() = delete;

        nonesuch(nonesuch const &) = delete;

        nonesuch(nonesuch &&) noexcept = delete;

        void operator=(nonesuch const &) = delete;

        void operator=(nonesuch &&) noexcept = delete;
    };

    namespace detail {
        template<class Default, class AlwaysVoid, template<class...> class Op, class... Args>
        struct detector {
            using value_t = std::false_type;
            using type = Default;
        };

        template<class Default, template<class...> class Op, class... Args>
        struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
            using value_t = std::true_type;
            using type = Op<Args...>;
        };

    } // namespace detail

    template<template<class...> class Op, class... Args>
    using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

    template<template<class...> class Op, class... Args>
    using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

    template<class Default, template<class...> class Op, class... Args>
    using detected_or = detail::detector<Default, void, Op, Args...>;

    template<class Expected, template<class...> class Op, class... Args>
    using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;
} // namespace AndreiUtils
