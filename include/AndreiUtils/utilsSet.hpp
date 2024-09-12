//
// Created by Andrei on 22.05.24.
//

#pragma once

#include <AndreiUtils/utilsMap.hpp>
#include <set>
#include <vector>

namespace AndreiUtils {
    template<typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    std::set<T, Compare, Alloc> setFromVector(std::vector<T> const &v) {
        return std::set<T, Compare, Alloc>(v.begin(), v.end());
    }

    template<typename T1, typename T2, typename Compare = std::less<T1>, typename SetAlloc = std::allocator<T1>,
            typename MapAlloc = std::allocator<std::pair<T1, T2>>>
    std::set<T1, Compare, SetAlloc> setFromMapKeys(std::map<T1, T2, Compare, MapAlloc> const &m) {
        return setFromVector(getMapKeys(m));
    }

    template<class T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
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

    template<class T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    void printSet(std::set<T, Compare, Alloc> const &x, std::function<std::string(T const &)> const &stringConversion,
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

    template<class T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    void printSet(std::set<T, Compare, Alloc> const &x,
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

    template<class T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    [[nodiscard]] std::string printSetToString(
            std::set<T, Compare, Alloc> const &x, std::string const &separator = ", ") {
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

    template<class T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    [[nodiscard]] std::string printSetToString(
            std::set<T, Compare, Alloc> const &x, std::function<std::string(T const &)> const &stringConversion,
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

    template<class T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    [[nodiscard]] std::string printSetToString(
            std::set<T, Compare, Alloc> const &x,
            std::function<std::string(T const &, size_t const &)> const &stringConversion,
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

    template<class T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    [[nodiscard]] std::set<T, Compare, Alloc> setIntersection(
            std::set<T, Compare, Alloc> const &s1, std::set<T, Compare, Alloc> const &s2) {
        std::set<T, Compare, Alloc> res;
        std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(res, res.begin()));
        return res;
    }

    template<class T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    [[nodiscard]] std::set<T, Compare, Alloc> setUnion(
            std::set<T, Compare, Alloc> const &s1, std::set<T, Compare, Alloc> const &s2) {
        std::set<T, Compare, Alloc> res;
        std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(res, res.begin()));
        return res;
    }
}