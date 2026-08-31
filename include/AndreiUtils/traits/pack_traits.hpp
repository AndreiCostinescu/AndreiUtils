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
// Created by Andrei on 10.05.24.
//

#pragma once

namespace AndreiUtils {
    template<class... T>
    struct GetFirstType {
        using Type = void;
    };

    template<class T>
    struct GetFirstType<T> {
        using Type = T;
    };

    template<class T1, class T2, class... TRest>
    struct GetFirstType<T1, T2, TRest...> {
        using Type = T1;
    };

    template<class... T>
    struct GetLastType {
        using Type = void;
    };

    template<class T>
    struct GetLastType<T> {
        using Type = T;
    };

    template<class T1, class T2, class... TRest>
    struct GetLastType<T1, T2, TRest...> {
        using Type = typename GetLastType<T2, TRest...>::Type;
    };
} // namespace AndreiUtils
