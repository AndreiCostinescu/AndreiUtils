//
// Created by Andrei on 20.11.23.
//

#pragma once

#include <complex>
#include <cstdint>
#include <string>
#include <sstream>
#include <type_traits>

namespace AndreiUtils {
    template<typename T>
    struct stringify {
        static std::string to_string(T const &datum) {
            std::stringstream ss;
            ss << datum;
            return ss.str();
        }
    };

    template<typename Type>
    struct stringify<Type &> {
        using T = Type &;

        static std::string to_string(T datum) {
            return stringify<Type>::to_string(datum);
        }
    };

    template<typename Type>
    struct stringify<Type const &> {
        using T = Type const &;

        static std::string to_string(T datum) {
            return stringify<Type>::to_string(datum);
        }
    };

    template<>
    struct stringify<bool> {
        using T = bool;

        static std::string to_string(T const &datum) {
            return datum ? "true" : "false";
        }
    };

    template<typename CType>
    struct stringify<std::complex<CType>> {
        using T = std::complex<CType>;

        static std::string to_string(T const &datum) {
            return std::to_string(datum.real()) + " + i * " + std::to_string(datum.imag());
        }
    };

    template<typename T1, typename T2>
    struct stringify<std::pair<T1, T2>> {
        static std::string to_string(std::pair<T1, T2> const &datum) {
            return "<" + stringify<T1>::to_string(datum.first) + ", " + stringify<T2>::to_string(datum.second) + ">";
        }
    };

    template<typename T>
    std::string toString(T &datum) {
        return stringify<T>::to_string(datum);
    }

    template<typename T>
    std::string toString(T const &datum) {
        return stringify<T>::to_string(datum);
    }
}
