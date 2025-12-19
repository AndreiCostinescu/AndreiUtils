//
// Created by Andrei on 19.12.25.
//

#pragma once

#include <concepts>

namespace AndreiUtils {
    template<typename T>
    concept NotAbstract = std::negation_v<std::is_abstract<T>>;
}