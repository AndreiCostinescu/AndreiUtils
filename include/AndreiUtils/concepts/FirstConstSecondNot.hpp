//
// Created by Andrei on 19.12.25.
//

#pragma once

#include <concepts>

namespace AndreiUtils {
    template<typename T1, typename T2>
    concept FirstConstSecondNot = (std::is_const_v<T1> && !std::is_const_v<T2>);
}