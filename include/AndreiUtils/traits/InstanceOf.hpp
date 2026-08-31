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
// Created by Andrei on 04.11.22.
//

#ifndef ANDREIUTILS_INSTANCEOF_HPP
#define ANDREIUTILS_INSTANCEOF_HPP

#include <type_traits>

namespace AndreiUtils {
    template<typename TypeCheck, typename InstanceType, bool IsInstanceTypePolymorphic>
    struct InstanceOf {
        static bool get(InstanceType const &) {
            return std::is_same_v<std::remove_const_t<TypeCheck>, std::remove_const_t<InstanceType>>;
        }

        static bool getIfIsInstance(InstanceType const &val, TypeCheck const *&res) {
            if constexpr (std::is_same_v<std::remove_const_t<TypeCheck>, std::remove_const_t<InstanceType>>) {
                res = &val;
                return true;
            } else {
                res = nullptr;
                return false;
            }
        }
    };

    template<typename TypeCheck, typename InstanceType>
    struct InstanceOf<TypeCheck, InstanceType *, false> {
        static bool get(InstanceType const *) {
            return std::is_same_v<std::remove_const_t<TypeCheck>, std::remove_const_t<InstanceType>>;
        }

        static bool getIfIsInstance(InstanceType const *val, TypeCheck const *&res) {
            if constexpr (std::is_same_v<std::remove_const_t<TypeCheck>, std::remove_const_t<InstanceType>>) {
                res = val;
                return true;
            } else {
                res = nullptr;
                return false;
            }
        }
    };

    template<typename TypeCheck, typename InstanceType>
    struct InstanceOf<TypeCheck, InstanceType, true> {
        static bool get(InstanceType const &val) { return dynamic_cast<TypeCheck const *>(&val) != nullptr; }

        static bool getIfIsInstance(InstanceType const &val, TypeCheck const *&res) {
            res = dynamic_cast<TypeCheck const *>(&val);
            return res != nullptr;
        }
    };

    template<typename TypeCheck, typename InstanceType>
    struct InstanceOf<TypeCheck, InstanceType *, true> {
        static bool get(InstanceType const *val) { return dynamic_cast<TypeCheck const *>(val) != nullptr; }

        static bool getIfIsInstance(InstanceType const *val, TypeCheck const *&res) {
            res = dynamic_cast<TypeCheck const *>(val);
            return res != nullptr;
        }
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_INSTANCEOF_HPP
