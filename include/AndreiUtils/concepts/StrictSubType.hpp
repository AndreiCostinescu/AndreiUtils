//
// Created by Andrei on 19.12.25.
//

#pragma once

#include <concepts>

namespace AndreiUtils {
    template<typename T, typename SubT>
    concept StrictSubType = std::is_base_of_v<std::remove_const_t<T>, std::remove_const_t<SubT>> &&
                            std::negation_v<std::is_same<std::remove_const_t<T>, std::remove_const_t<SubT>>>;
}
