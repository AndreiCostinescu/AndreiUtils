//
// Created by Andrei on 14.05.26.
//

#pragma once

#include <type_traits>

namespace AndreiUtils {
    // check whether T is an instantiation of template C
    template <typename, template <class...> class> struct is_template_instantiation_of : std::false_type {};

    template <template <class...> class C, class... Args>
    struct is_template_instantiation_of<C<Args...>, C> : std::true_type {};

    template <typename T, template <class...> class C>
    inline constexpr bool is_template_instantiation_of_v = is_template_instantiation_of<T, C>::value;
}
