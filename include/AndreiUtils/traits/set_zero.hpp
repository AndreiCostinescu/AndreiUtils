//
// Created by Andrei on 31.03.22.
//

#ifndef ANDREIUTILS_SET_ZERO_HPP
#define ANDREIUTILS_SET_ZERO_HPP

namespace AndreiUtils {
    template<typename T>
    struct set_zero {
        static void setZero(T &x) {
            x = 0;
        }
    };
}

#endif //ANDREIUTILS_SET_ZERO_HPP
