//
// Created by andrei on 31.03.22.
//

#ifndef ANDREIUTILS_IS_EIGEN_DATA_H
#define ANDREIUTILS_IS_EIGEN_DATA_H

namespace AndreiUtils {
    template<typename T>
    struct is_eigen_data {
        static const bool value = false;
    };
}

#endif //ANDREIUTILS_IS_EIGEN_DATA_H
