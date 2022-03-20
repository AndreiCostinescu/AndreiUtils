//
// Created by Andrei on 20-Mar-22.
//

#ifndef ANDREIUTILS_CONTAINER2D_HPP
#define ANDREIUTILS_CONTAINER2D_HPP

#include <stdexcept>

namespace AndreiUtils {
    template<typename T>
    class Container2D {
    public:
        static const bool isContainer2D = false;
    };

    template<typename T>
    class ModifiableContainer2D : public Container2D<T> {
    };
}

#endif //ANDREIUTILS_CONTAINER2D_HPP
