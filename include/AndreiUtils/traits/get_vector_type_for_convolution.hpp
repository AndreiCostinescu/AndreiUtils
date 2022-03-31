//
// Created by Andrei on 20-Mar-22.
//

#ifndef ANDREIUTILS_GET_VECTOR_TYPE_FOR_CONVOLUTION_HPP
#define ANDREIUTILS_GET_VECTOR_TYPE_FOR_CONVOLUTION_HPP

namespace AndreiUtils {
    template<typename T>
    struct get_vector_type_for_convolution {
        typedef T type;
    };
}

#endif //ANDREIUTILS_GET_VECTOR_TYPE_FOR_CONVOLUTION_HPP
