//
// Created by andrei on 24.03.22.
//

#ifndef ANDREIUTILS_MAPUTILS_HPP
#define ANDREIUTILS_MAPUTILS_HPP

#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace AndreiUtils {
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    bool mapContains(const std::map<T1, T2, C, A> &container, const T1 &key, T2 *value = nullptr) {
        const auto &data = container.find(key);
        if (data != container.end()) {
            if (value != nullptr) {
                *value = data->second;
            }
            return true;
        }
        return false;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    bool mapContains(const std::map<T1 *, T2, C, A> &container, const T1 *key, T2 *value = nullptr) {
        const auto &data = container.find(const_cast<T1 *>(key));
        if (data != container.end()) {
            if (value != nullptr) {
                *value = data->second;
            }
            return true;
        }
        return false;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    bool mapGetIfContains(const std::map<T1, T2, C, A> &container, const T1 &key, T2 &value) {
        return mapContains(container, key, &value);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    bool mapGetIfContains(const std::map<T1 *, T2, C, A> &container, const T1 *key, T2 &value) {
        return mapContains(container, key, &value);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    T2 mapGet(const std::map<T1, T2, C, A> &container, const T1 &key) {
        T2 value;
        if (!mapContains(container, key, &value)) {
            throw std::runtime_error("Element not found in map!");
        }
        return value;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    T2 mapGet(const std::map<T1 *, T2, C, A> &container, const T1 *key) {
        T2 value;
        if (!mapContains(container, key, &value)) {
            throw std::runtime_error("Element not found in map!");
        }
        return value;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    T2 &mapGetRef(std::map<T1, T2, C, A> &container, const T1 &key) {
        if (!mapContains(container, key)) {
            throw std::runtime_error("Element not found in map!");
        }
        return container.at(key);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    T2 &mapGetRef(std::map<T1 *, T2, C, A> &container, const T1 *key) {
        if (!mapContains(container, key)) {
            throw std::runtime_error("Element not found in map!");
        }
        return container.at(const_cast<T1 *>(key));
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    const T2 &mapGetRef(const std::map<T1, T2, C, A> &container, const T1 &key) {
        if (!mapContains(container, key)) {
            throw std::runtime_error("Element not found in map!");
        }
        return container.at(key);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    const T2 &mapGetRef(const std::map<T1 *, T2, C, A> &container, const T1 *key) {
        if (!mapContains(container, key)) {
            throw std::runtime_error("Element not found in map!");
        }
        return container.at(const_cast<T1 *>(key));
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void mapSet(std::map<T1, T2, C, A> &container, const T1 &key, const T2 &value) {
        container[key] = value;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    void mapSet(std::map<T1 *, T2, C, A> &container, const T1 *key, const T2 &value) {
        container[const_cast<T1 *>(key)] = value;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    bool mapDelete(std::map<T1, T2, C, A> &container, const T1 &key) {
        return (container.erase(key) > 0);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1 *, T2>>>
    bool mapDelete(std::map<T1 *, T2, C, A> &container, const T1 *key) {
        return (container.erase(const_cast<T1 *>(key)) > 0);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMap(const std::map<T1, T2, C, A> &container) {
        for (const auto &containerItem: container) {
            std::cout << containerItem.first << " -> " << containerItem.second << std::endl;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMap(const std::map<T1, T2, C, A> &container,
                  const std::function<std::string(T2 const &)> &stringConversion) {
        for (const auto &containerItem: container) {
            std::cout << containerItem.first << " -> " << stringConversion(containerItem.second) << std::endl;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::vector<T1> getMapKeys(const std::map<T1, T2, C, A> &container) {
        std::vector<T1> keys(container.size());
        int i = 0;
        for (const auto &elem: container) {
            keys[i++] = elem.first;
        }
        return keys;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    std::vector<T2> getMapValues(const std::map<T1, T2, C, A> &container) {
        std::vector<T2> values(container.size());
        int i = 0;
        for (const auto &elem: container) {
            values[i++] = elem.second;
        }
        return values;
    }

    template<class T1, class T2>
    std::map<T1, T2> createMapFromKeysAndValues(const std::vector<T1> &keys, const std::vector<T2> &values) {
        assert (keys.size() == values.size());
        std::map<T1, T2> res;
        for (int i = 0; i < keys.size(); i++) {
            res[keys[i]] = values[i];
        }
        return res;
    }
}

#endif //ANDREIUTILS_MAPUTILS_HPP
