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
    };

    template<typename TypeCheck, typename InstanceType>
    struct InstanceOf<TypeCheck, InstanceType *, false> {
        static bool get(InstanceType const *) {
            return std::is_same_v<std::remove_const_t<TypeCheck>, std::remove_const_t<InstanceType>>;
        }
    };

    template<typename TypeCheck, typename InstanceType>
    struct InstanceOf<TypeCheck, InstanceType, true> {
        static bool get(InstanceType const &val) {
            return dynamic_cast<TypeCheck const *>(&val) != nullptr;
        }
    };

    template<typename TypeCheck, typename InstanceType>
    struct InstanceOf<TypeCheck, InstanceType *, true> {
        static bool get(InstanceType const *val) {
            return dynamic_cast<TypeCheck const *>(val) != nullptr;
        }
    };
}

#endif //ANDREIUTILS_INSTANCEOF_HPP
