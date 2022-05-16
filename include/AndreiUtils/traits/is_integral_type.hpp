//
// Created by andrei on 13.05.22.
//

#ifndef ANDREIUTILS_IS_INTEGRAL_DATA_H
#define ANDREIUTILS_IS_INTEGRAL_DATA_H

#include <cstdint>

namespace AndreiUtils {
    template<typename T>
    struct is_integral_type {
        static const bool value = false;
    };

    template<>
    struct is_integral_type<bool> {
        static const bool value = true;
    };

    template<>
    struct is_integral_type<std::int8_t> {
        static const bool value = true;
    };

    template<>
    struct is_integral_type<std::uint8_t> {
        static const bool value = true;
    };

    template<>
    struct is_integral_type<std::int16_t> {
        static const bool value = true;
    };

    template<>
    struct is_integral_type<std::uint16_t> {
        static const bool value = true;
    };

    template<>
    struct is_integral_type<std::int32_t> {
        static const bool value = true;
    };

    template<>
    struct is_integral_type<std::uint32_t> {
        static const bool value = true;
    };

    template<>
    struct is_integral_type<std::int64_t> {
        static const bool value = true;
    };

    template<>
    struct is_integral_type<std::uint64_t> {
        static const bool value = true;
    };

    template<>
    struct is_integral_type<long long> {
        static const bool value = true;
    };

    template<>
    struct is_integral_type<unsigned long long> {
        static const bool value = true;
    };
}

#endif //ANDREIUTILS_IS_INTEGRAL_DATA_H
