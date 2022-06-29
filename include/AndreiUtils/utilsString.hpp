//
// Created by Andrei Costinescu on 23.06.22.
//

#ifndef ANDREIUTILS_UTILSSTRING_HPP
#define ANDREIUTILS_UTILSSTRING_HPP

#include <functional>
#include <string>

namespace AndreiUtils {
    template<typename T>
    std::string stringify(T const &t) {
        return std::to_string(t);
    }

    template<>
    std::string stringify<std::string>(std::string const &t) {
        return t;
    }
}

#endif //ANDREIUTILS_UTILSSTRING_HPP
