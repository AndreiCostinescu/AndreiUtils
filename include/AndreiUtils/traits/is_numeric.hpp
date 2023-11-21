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
}
