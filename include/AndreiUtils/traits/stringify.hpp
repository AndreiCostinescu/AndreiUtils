// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 20.11.23.
//

#pragma once

#include <AndreiUtils/utilsVector.hpp>
#include <algorithm>
#include <complex>
#include <cstdint>
#include <sstream>
#include <string>
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

    template<typename T>
    std::string toString(T &datum) {
        return stringify<T>::to_string(datum);
    }

    template<typename T>
    std::string toString(T const &datum) {
        return stringify<T>::to_string(datum);
    }

    template<typename Type>
    struct stringify<Type &> {
        using T = Type &;

        static std::string to_string(T datum) { return stringify<Type>::to_string(datum); }
    };

    template<typename Type>
    struct stringify<Type const &> {
        using T = Type const &;

        static std::string to_string(T datum) { return stringify<Type>::to_string(datum); }
    };

    template<>
    struct stringify<std::string> {
        using T = std::string;

        static std::string to_string(T const &datum) { return datum; }
    };

    template<>
    struct stringify<bool> {
        using T = bool;

        static std::string to_string(T const &datum) { return datum ? "true" : "false"; }
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

    template<typename... TArgs>
    struct stringify<std::tuple<TArgs...>> {
    public:
        static std::string to_string(std::tuple<TArgs...> const &datum) {
            std::vector<std::string> res;
            std::apply([&res](auto &&...args) { ((res.emplace_back(toString(args))), ...); }, datum);
            return "(" + AndreiUtils::printVectorToString(res) + ")";
        }
    };

    template<typename T>
    struct stringify<std::vector<T>> {
        static std::string to_string(std::vector<T> const &datum) {
            return "[" + AndreiUtils::printVectorToString(datum) + "]";
        }
    };
} // namespace AndreiUtils
