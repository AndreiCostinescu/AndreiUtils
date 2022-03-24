//
// Created by Andrei on 20-Mar-22.
//

#ifndef ANDREIUTILS_MEDIAN_COMPUTER_HPP
#define ANDREIUTILS_MEDIAN_COMPUTER_HPP

#include <AndreiUtils/utilsVector.hpp>

template<typename T>
struct median_computer {
    static T medianComputer(std::vector<T> v) {
        return AndreiUtils::median(v);
    }
};

#endif //ANDREIUTILS_MEDIAN_COMPUTER_HPP
