//
// Created by Andrei on 19.12.25.
//

#pragma once

#include <concepts>

namespace AndreiUtils {
    template<typename T>
    concept NotConst = std::negation_v<std::is_const<T>>;
}