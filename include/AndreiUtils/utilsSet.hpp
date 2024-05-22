//
// Created by Andrei on 22.05.24.
//

#pragma once

#include <AndreiUtils/utilsMap.hpp>
#include <set>
#include <vector>

namespace AndreiUtils {
    template<typename T>
    std::set<T> setFromVector(std::vector<T> const &v) {
        return std::set<T>(v.begin(), v.end());
    }

    template<typename T1, typename T2>
    std::set<T1> setFromMapKeys(std::map<T1, T2> const &m) {
        return setFromVector(getMapKeys(m));
    }

    template<class T>
    void printSet(std::set<T> const &x, std::string const &separator = ", ") {
        bool firstPrint = true;
        for (auto const &elem: x) {
            if (!firstPrint) {
                std::cout << separator;
            }
            std::cout << elem;
            firstPrint = false;
        }
        std::cout << std::endl;
    }

    template<class T>
    void printSet(std::set<T> const &x, std::function<std::string(T const &)> const &stringConversion,
                  std::string const &separator = ", ") {
        bool firstPrint = true;
        for (auto const &elem: x) {
            if (!firstPrint) {
                std::cout << separator;
            }
            std::cout << stringConversion(elem);
            firstPrint = false;
        }
        std::cout << std::endl;
    }

    template<class T>
    void printSet(std::set<T> const &x,
                  std::function<std::string(T const &, size_t const &index)> const &stringConversion,
                  std::string const &separator = ", ") {
        int index = 0;
        for (auto const &elem: x) {
            if (index > 0) {
                std::cout << separator;
            }
            std::cout << stringConversion(elem, index);
            ++index;
        }
        std::cout << std::endl;
    }

    template<class T>
    [[nodiscard]] std::string printSetToString(std::set<T> const &x, std::string const &separator = ", ") {
        std::stringstream s;
        bool firstPrint = true;
        for (auto const &elem: x) {
            if (!firstPrint) {
                s << separator;
            }
            s << elem;
            firstPrint = false;
        }
        return s.str();
    }

    template<class T>
    [[nodiscard]] std::string printSetToString(
            std::set<T> const &x, std::function<std::string(T const &)> const &stringConversion,
            std::string const &separator = ", ") {
        std::stringstream s;
        bool firstPrint = true;
        for (auto const &elem: x) {
            if (!firstPrint) {
                s << separator;
            }
            s << stringConversion(elem);
            firstPrint = false;
        }
        return s.str();
    }

    template<class T>
    [[nodiscard]] std::string printSetToString(
            std::set<T> const &x, std::function<std::string(T const &, size_t const &)> const &stringConversion,
            std::string const &separator = ", ") {
        std::stringstream s;
        int index = 0;
        for (auto const &elem: x) {
            if (index > 0) {
                s << separator;
            }
            s << stringConversion(elem, index);
            ++index;
        }
        return s.str();
    }
}