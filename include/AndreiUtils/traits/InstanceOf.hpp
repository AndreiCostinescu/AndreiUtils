//
// Created by Andrei on 04.11.22.
//

#ifndef ANDREIUTILS_INSTANCEOF_HPP
#define ANDREIUTILS_INSTANCEOF_HPP

#include <type_traits>

namespace AndreiUtils {
    template<typename TypeCheck, typename InstanceType, bool IsInstanceTypePolymorphic>
    struct InstanceOf {
        static bool get(InstanceType const &val) {
            return false;
        }
    };

    template<typename T, typename InstanceType>
    struct InstanceOf<T, InstanceType, true> {
        static bool get(InstanceType const &val) {
            return dynamic_cast<T const *>(&val) != nullptr;
        }
    };

    template<typename T, typename InstanceType>
    struct InstanceOf<T, InstanceType *, true> {
        static bool get(InstanceType const *val) {
            return dynamic_cast<T const *>(val) != nullptr;
        }
    };
}
#endif //ANDREIUTILS_INSTANCEOF_HPP
