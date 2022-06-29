//
// Created by andrei on 24.03.22.
//

#ifndef ANDREIUTILS_MAPUTILS_HPP
#define ANDREIUTILS_MAPUTILS_HPP

#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

namespace AndreiUtils {
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const, T2>>>
    bool mapContains(std::map<T1, T2, C, A> const &container, T1 const &key) {
        const auto &data = container.find(key);
        return data != container.end();
    }

    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    bool mapContains(std::map<T1 *, T2, C, A> const &container, T1 const *key) {
        const auto &data = container.find(const_cast<T1 *>(key));
        return data != container.end();
    }

    // if element is found, gets a copy of the element's value in the variable value
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    bool mapGetIfContains(std::map<T1, T2, C, A> const &container, T1 const &key, T2 &value) {
        const auto &data = container.find(key);
        if (data != container.end()) {
            value = data->second;
            return true;
        }
        return false;
    }

    // if element is found, gets a (non-const) pointer-reference of the element in the variable value
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const, T2>>>
    bool mapGetIfContains(std::map<T1, T2, C, A> &container, T1 const &key, T2 *&value) {
        auto data = container.find(key);
        if (data != container.end()) {
            value = &data->second;
            return true;
        }
        return false;
    }

    // if element is found, gets a (const) pointer-reference of the element in the variable value
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    bool mapGetIfContains(std::map<T1, T2, C, A> const &container, T1 const &key, T2 const *&value) {
        const auto &data = container.find(key);
        if (data != container.end()) {
            value = &data->second;
            return true;
        }
        return false;
    }

    // if element (having a pointer as key) is found, gets a copy of the element's value in the variable value
    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    bool mapGetIfContains(std::map<T1 *, T2, C, A> const &container, T1 const *const &key, T2 &value) {
        const auto &data = container.find(const_cast<T1 *>(key));
        if (data != container.end()) {
            value = data->second;
            return true;
        }
        return false;
    }

    // if element (having a pointer as key) is found, gets a (non-const) pointer-reference of the element in the variable value
    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    bool mapGetIfContains(std::map<T1 *, T2, C, A> &container, T1 const *const &key, T2 *&value) {
        auto data = container.find(const_cast<T1 *>(key));
        if (data != container.end()) {
            value = &data->second;
            return true;
        }
        return false;
    }

    // if element (having a pointer as key) is found, gets a (const) pointer-reference of the element in the variable value
    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    bool mapGetIfContains(std::map<T1 *, T2, C, A> const &container, T1 const *const &key, T2 const *&value) {
        const auto &data = container.find(const_cast<T1 *>(key));
        if (data != container.end()) {
            value = &data->second;
            return true;
        }
        return false;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    T2 &mapGet(std::map<T1, T2, C, A> &container, T1 const &key) {
        auto data = container.find(key);
        if (data == container.end()) {
            throw std::runtime_error("Element not found in map!");
        }
        return data->second;
    }

    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    T2 &mapGet(std::map<T1 *, T2, C, A> &container, T1 const *key) {
        auto data = container.find(const_cast<T1 *>(key));
        if (data == container.end()) {
            throw std::runtime_error("Element not found in map!");
        }
        return data->second;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    T2 const &mapGet(std::map<T1, T2, C, A> const &container, T1 const &key) {
        const auto &data = container.find(key);
        if (data == container.end()) {
            throw std::runtime_error("Element not found in map!");
        }
        return data->second;
    }

    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    T2 const &mapGet(std::map<T1 *, T2, C, A> const &container, T1 const *key) {
        auto data = container.find(const_cast<T1 *>(key));
        if (data == container.end()) {
            throw std::runtime_error("Element not found in map!");
        }
        return data->second;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void mapSet(std::map<T1, T2, C, A> &container, T1 const &key, T2 const &value) {
        container[key] = value;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    void mapSet(std::map<T1 *, T2, C, A> &container, T1 const *key, T2 const &value) {
        container[const_cast<T1 *>(key)] = value;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void mapAdd(std::map<T1, T2, C, A> &container, T1 const &key, T2 const &value) {
        if (mapContains(container, key)) {
            throw std::runtime_error("Key already is in container!");
        }
        container.insert({key, value});
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    void mapAdd(std::map<T1 *, T2, C, A> &container, T1 const *key, T2 const &value) {
        if (mapContains(container, key)) {
            throw std::runtime_error("Key already is in container!");
        }
        container.insert({const_cast<T1 *>(key), value});
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    bool mapDelete(std::map<T1, T2, C, A> &container, T1 const &key) {
        return (container.erase(key) > 0);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    bool mapDelete(std::map<T1 *, T2, C, A> &container, T1 const *key) {
        return (container.erase(const_cast<T1 *>(key)) > 0);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMap(std::map<T1, T2, C, A> const &container) {
        for (const auto &containerItem: container) {
            std::cout << containerItem.first << " -> " << containerItem.second << std::endl;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMap(std::map<T1, T2, C, A> const &container,
                  std::function<std::string(T1 const &)> const &keyStringConversion,
                  std::function<std::string(T2 const &)> const &valueStringConversion) {
        for (const auto &containerItem: container) {
            std::cout << keyStringConversion(containerItem.first) << " -> "
                      << valueStringConversion(containerItem.second) << std::endl;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMapConvertKey(std::map<T1, T2, C, A> const &container,
                            std::function<std::string(T1 const &)> const &keyStringConversion) {
        for (const auto &containerItem: container) {
            std::cout << keyStringConversion(containerItem.first) << " -> " << containerItem.second << std::endl;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMapConvertValue(std::map<T1, T2, C, A> const &container,
                              std::function<std::string(T2 const &)> const &valueStringConversion) {
        for (const auto &containerItem: container) {
            std::cout << containerItem.first << " -> " << valueStringConversion(containerItem.second) << std::endl;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::string printMapToString(std::map<T1, T2, C, A> const &container) {
        std::stringstream ss;
        for (const auto &containerItem: container) {
            ss << containerItem.first << " -> " << containerItem.second << std::endl;
        }
        return ss.str();
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::string printMapToString(std::map<T1, T2, C, A> const &container,
                                 std::function<std::string(T1 const &)> const &keyStringConversion,
                                 std::function<std::string(T2 const &)> const &valueStringConversion) {
        std::stringstream ss;
        for (const auto &containerItem: container) {
            ss << keyStringConversion(containerItem.first) << " -> " << valueStringConversion(containerItem.second)
               << std::endl;
        }
        return ss.str();
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::string printMapToStringConvertKey(std::map<T1, T2, C, A> const &container,
                                           std::function<std::string(T1 const &)> const &keyStringConversion) {
        std::stringstream ss;
        for (const auto &containerItem: container) {
            ss << keyStringConversion(containerItem.first) << " -> " << containerItem.second << std::endl;
        }
        return ss.str();
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::string printMapToStringConvertValue(std::map<T1, T2, C, A> const &container,
                                             std::function<std::string(T2 const &)> const &valueStringConversion) {
        std::stringstream ss;
        for (const auto &containerItem: container) {
            ss << containerItem.first << " -> " << valueStringConversion(containerItem.second) << std::endl;
        }
        return ss.str();
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::vector<T1> getMapKeys(std::map<T1, T2, C, A> const &container) {
        std::vector<T1> keys(container.size());
        int i = 0;
        for (const auto &elem: container) {
            keys[i++] = elem.first;
        }
        return keys;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::vector<T2> getMapValues(std::map<T1, T2, C, A> const &container) {
        std::vector<T2> values(container.size());
        int i = 0;
        for (const auto &elem: container) {
            values[i++] = elem.second;
        }
        return values;
    }

    template<class T1, class T2>
    std::map<T1, T2> createMapFromKeysAndValues(std::vector<T1> const &keys, std::vector<T2> const &values) {
        assert(keys.size() == values.size());
        std::map<T1, T2> res;
        for (int i = 0; i < keys.size(); i++) {
            res[keys[i]] = values[i];
        }
        return res;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void filterMapBasedOnPredicate(std::map<T1, T2, C, A> &container,
                                   std::function<bool(T1 const &, T2 const &)> const &predicate) {
        for (auto elem = container.begin(); elem != container.end();) {
            if (predicate(elem->first, elem->second)) {
                elem = container.erase(elem);
            } else {
                ++elem;
            }
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::map<T1, T2, C, A> getFilteredMapBasedOnPredicate(
            std::map<T1, T2, C, A> const &container, std::function<bool(T1 const &, T2 const &)> const &predicate) {
        std::map<T1, T2, C, A> result = container;
        filterMapBasedOnPredicate(result, predicate);
        return result;
    }

    template<class T, typename A = std::allocator<std::pair<const int, T>>>
    std::map<int, T, std::less<int>, A> mapFromVector(std::vector<T> const &v) {
        std::map<int, T, std::less<int>, A> m;
        for (size_t i = 0; i < v.size(); i++) {
            m[i] = v[i];
        }
        return m;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::map<T1, T2, C, A> mapFromVector(std::vector<T2> const &v, std::function<T1(T2 const &)> const &keyOp) {
        std::map<T1, T2, C, A> m;
        for (size_t i = 0; i < v.size(); i++) {
            m[keyOp(v[i])] = v[i];
        }
        return m;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::map<T1, T2, C, A> mapFromVector(std::vector<T2> const &v, std::function<T1(T2 const &, size_t)> const &keyOp) {
        std::map<T1, T2, C, A> m;
        for (size_t i = 0; i < v.size(); i++) {
            m[keyOp(v[i], i)] = v[i];
        }
        return m;
    }
}

#endif //ANDREIUTILS_MAPUTILS_HPP
