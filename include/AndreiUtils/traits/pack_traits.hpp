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
}