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
}
