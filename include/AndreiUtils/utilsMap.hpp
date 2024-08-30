//
// Created by Andrei on 24.03.22.
//

#pragma once

#include <AndreiUtils/classes/RandomNumberGenerator.hpp>
#include <AndreiUtils/traits/stringify.hpp>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

namespace AndreiUtils {
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const, T2>>>
    [[nodiscard]] bool mapContains(std::map<T1, T2, C, A> const &container, T1 const &key) {
        auto const &data = container.find(key);
        return data != container.end();
    }

    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    [[nodiscard]] bool mapContains(std::map<T1 *, T2, C, A> const &container, T1 const *key) {
        auto const &data = container.find(const_cast<T1 *>(key));
        return data != container.end();
    }

    // if element is found, gets a copy of the element's value in the variable value
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    bool mapGetIfContains(std::map<T1, T2, C, A> const &container, T1 const &key, T2 &value) {
        auto const &data = container.find(key);
        if (data != container.end()) {
            value = data->second;  // copy data
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
        auto const &data = container.find(key);
        if (data != container.end()) {
            value = &data->second;
            return true;
        }
        return false;
    }

    // if element (having a pointer as key) is found, gets a copy of the element's value in the variable value
    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    bool mapGetIfContains(std::map<T1 *, T2, C, A> const &container, T1 const *const &key, T2 &value) {
        auto const &data = container.find(const_cast<T1 *>(key));
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
        auto const &data = container.find(const_cast<T1 *>(key));
        if (data != container.end()) {
            value = &data->second;
            return true;
        }
        return false;
    }

    // if element is found, gets a copy of the element's value in the variable value
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2 *>>>
    bool mapGetIfContains(std::map<T1, T2 *, C, A> const &container, T1 const &key, T2 &value) {
        auto const &data = container.find(key);
        if (data != container.end()) {
            value = *data->second;  // copy data
            return true;
        }
        return false;
    }

    // if element is found, gets a copy of the element's value in the variable value
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2 const *>>>
    bool mapGetIfContains(std::map<T1, T2 const *, C, A> const &container, T1 const &key, T2 &value) {
        auto const &data = container.find(key);
        if (data != container.end()) {
            value = *data->second;  // copy data
            return true;
        }
        return false;
    }

    // if element is found, gets a const pointer-copy of the element in the variable value
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2 *>>>
    bool mapGetIfContains(std::map<T1, T2 *, C, A> const &container, T1 const &key, T2 const *&value) {
        auto const &data = container.find(key);
        if (data != container.end()) {
            value = data->second;
            return true;
        }
        return false;
    }

    // if element (having a pointer as key) is found, gets a copy of the element's value in the variable value
    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2 *>>>
    bool mapGetIfContains(std::map<T1 *, T2 *, C, A> const &container, T1 const *const &key, T2 &value) {
        auto const &data = container.find(const_cast<T1 *>(key));
        if (data != container.end()) {
            value = *data->second;
            return true;
        }
        return false;
    }

    // if element (having a pointer as key) is found, gets a copy of the element's value in the variable value
    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2 const *>>>
    bool mapGetIfContains(std::map<T1 *, T2 const *, C, A> const &container, T1 const *const &key, T2 &value) {
        auto const &data = container.find(const_cast<T1 *>(key));
        if (data != container.end()) {
            value = *data->second;
            return true;
        }
        return false;
    }

    // if element (having a pointer as key) is found, gets a const pointer-copy of the element in the variable value
    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2 *>>>
    bool mapGetIfContains(std::map<T1 *, T2 *, C, A> const &container, T1 const *const &key, T2 const *&value) {
        auto const &data = container.find(const_cast<T1 *>(key));
        if (data != container.end()) {
            value = data->second;
            return true;
        }
        return false;
    }

    // SHARED_PTR: if element is found, gets a const pointer-copy of the element in the variable value
    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2 *>>>
    bool mapGetIfContains(std::map<T1, std::shared_ptr<T2>, C, A> const &container, T1 const &key,
                          std::shared_ptr<T2 const> &value) {
        auto const &data = container.find(key);
        if (data != container.end()) {
            value = data->second;
            return true;
        }
        return false;
    }

    // SHARED_PTR: if element (having a pointer as key) is found, gets a const pointer-copy of the element in the variable value
    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2 *>>>
    bool mapGetIfContains(std::map<T1 *, std::shared_ptr<T2>, C, A> const &container, T1 const *const &key,
                          std::shared_ptr<T2 const> &value) {
        auto const &data = container.find(const_cast<T1 *>(key));
        if (data != container.end()) {
            value = data->second;
            return true;
        }
        return false;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] T2 &mapGet(std::map<T1, T2, C, A> &container, T1 const &key) {
        auto data = container.find(key);
        if (data == container.end()) {
            throw std::runtime_error("Element " + AndreiUtils::toString(key) + " not found in map!");
        }
        return data->second;
    }

    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    [[nodiscard]] T2 &mapGet(std::map<T1 *, T2, C, A> &container, T1 const *key) {
        auto data = container.find(const_cast<T1 *>(key));
        if (data == container.end()) {
            throw std::runtime_error("Element " + AndreiUtils::toString(key) + " not found in map!");
        }
        return data->second;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] T2 const &mapGet(std::map<T1, T2, C, A> const &container, T1 const &key) {
        auto const &data = container.find(key);
        if (data == container.end()) {
            throw std::runtime_error("Element " + AndreiUtils::toString(key) + " not found in map!");
        }
        return data->second;
    }

    template<class T1, class T2, typename C = std::less<T1 *>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    [[nodiscard]] T2 const &mapGet(std::map<T1 *, T2, C, A> const &container, T1 const *key) {
        auto data = container.find(const_cast<T1 *>(key));
        if (data == container.end()) {
            throw std::runtime_error("Element " + AndreiUtils::toString(key) + " not found in map!");
        }
        return data->second;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    typename std::map<T1, T2, C, A>::iterator
    mapSet(std::map<T1, T2, C, A> &container, T1 const &key, T2 const &value) {
        return container.insert_or_assign(key, value).first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    typename std::map<T1, T2, C, A>::iterator
    mapSet(std::map<T1 *, T2, C, A> &container, T1 const *key, T2 const &value) {
        return container.insert_or_assign(const_cast<T1 *>(key), value).first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    typename std::map<T1, T2, C, A>::iterator mapSet(std::map<T1, T2, C, A> &container, T1 const &key, T2 &&value) {
        return container.insert_or_assign(key, std::forward<T2>(value)).first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    typename std::map<T1, T2, C, A>::iterator mapSet(std::map<T1 *, T2, C, A> &container, T1 const *key, T2 &&value) {
        return container.insert_or_assign(const_cast<T1 *>(key), std::forward<T2>(value)).first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    typename std::map<T1, T2, C, A>::iterator mapAdd(std::map<T1, T2, C, A> &container, T1 const &key,
                                                     T2 const &value) {
        auto x = container.insert({key, value});
        if (!x.second) {
            throw std::runtime_error("Key " + AndreiUtils::toString(key) + " already is in container!");
        }
        return x.first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    typename std::map<T1, T2, C, A>::iterator mapAdd(std::map<T1, T2, C, A> &container, T1 const &key, T2 &&value) {
        auto x = container.insert(std::make_pair(key, std::forward<T2>(value)));
        if (!x.second) {
            throw std::runtime_error("Key " + AndreiUtils::toString(key) + " already is in container!");
        }
        return x.first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    typename std::map<T1, T2, C, A>::iterator mapAdd(std::map<T1 *, T2, C, A> &container, T1 const *key,
                                                     T2 const &value) {
        auto x = container.insert({const_cast<T1 *>(key), value});
        if (!x.second) {
            throw std::runtime_error("Key " + AndreiUtils::toString(key) + " already is in container!");
        }
        return x.first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const *, T2>>>
    typename std::map<T1, T2, C, A>::iterator mapAdd(std::map<T1 *, T2, C, A> &container, T1 const *key, T2 &&value) {
        auto x = container.insert(std::make_pair(const_cast<T1 *>(key), std::forward<T2>(value)));
        if (!x.second) {
            throw std::runtime_error("Key " + AndreiUtils::toString(key) + " already is in container!");
        }
        return x.first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const, T2>>, typename... Args>
    typename std::map<T1, T2, C, A>::iterator mapEmplace(std::map<T1, T2, C, A> &container, T1 const &key,
                                                         Args &&... args) {
        auto x = container.emplace(std::piecewise_construct, std::forward_as_tuple(key),
                                   std::forward_as_tuple(std::forward<Args>(args)...));
        if (!x.second) {
            throw std::runtime_error("Key " + AndreiUtils::toString(key) + " already is in container!");
        }
        return x.first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const *, T2>>, typename... Args>
    typename std::map<T1, T2, C, A>::iterator mapEmplace(std::map<T1, T2, C, A> &container, T1 const *key,
                                                         Args &&... args) {
        auto x = container.emplace(std::piecewise_construct, std::forward_as_tuple(*key),
                                   std::forward_as_tuple(std::forward<Args>(args)...));
        if (!x.second) {
            throw std::runtime_error("Key " + AndreiUtils::toString(*key) + " already is in container!");
        }
        return x.first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const, T2>>, typename... Args>
    typename std::map<T1, T2, C, A>::iterator mapEmplace(std::map<T1, T2, C, A> &container, T1 &&key, Args &&... args) {
        // set the text here before the move happens in the container.emplace line!
        std::string textIfEmplaceFails = "Key " + AndreiUtils::toString(key) + " already is in container!";
        auto x = container.emplace(std::piecewise_construct, std::forward_as_tuple(std::forward<T1>(key)),
                                   std::forward_as_tuple(std::forward<Args>(args)...));
        if (!x.second) {
            throw std::runtime_error(textIfEmplaceFails);
        }
        return x.first;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const, T2>>, typename... Args>
    T2 *mapAddIfNotContains(std::map<T1, T2, C, A> &container, T1 const &key, Args &&... args) {
        T2 *res;
        if (!mapGetIfContains(container, key, res)) {
            return &(mapEmplace(container, key, std::forward<Args>(args)...)->second);
        }
        return res;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<T1 const *, T2>>, typename... Args>
    T2 *mapAddIfNotContains(std::map<T1 *, T2, C, A> &container, T1 const *key, Args &&... args) {
        T2 *res;
        if (!mapGetIfContains(container, const_cast<T1 *>(key), res)) {
            return &(mapEmplace(container, const_cast<T1 *>(key), std::forward<Args>(args)...)->second);
        }
        return res;
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
    void printMap(std::map<T1, T2, C, A> const &container,
                  std::function<bool(T1 const &, T2 const &)> const &elementFilter,
                  std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n",
                  bool withNewline = true) {
        bool firstItem = true;
        for (auto const &containerItem: container) {
            if (elementFilter && elementFilter(containerItem.first, containerItem.second)) {
                continue;
            }
            if (!firstItem) {
                std::cout << itemSeparator;
            }
            std::cout << containerItem.first << keyValueSeparator << containerItem.second;
            firstItem = false;
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMap(std::map<T1, T2, C, A> const &container, std::string const &keyValueSeparator = " -> ",
                  std::string const &itemSeparator = "\n", bool withNewline = true) {
        printMap(container, {}, keyValueSeparator, itemSeparator, withNewline);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMap(std::map<T1, T2, C, A> const &container,
                  std::function<bool(T1 const &, T2 const &)> const &elementFilter,
                  std::function<std::string(T1 const &)> const &keyStringConversion,
                  std::function<std::string(T2 const &)> const &valueStringConversion,
                  std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n",
                  bool withNewline = true) {
        bool firstItem = true;
        for (auto const &containerItem: container) {
            if (elementFilter && elementFilter(containerItem.first, containerItem.second)) {
                continue;
            }
            if (!firstItem) {
                std::cout << itemSeparator;
            }
            std::cout << keyStringConversion(containerItem.first) << keyValueSeparator
                      << valueStringConversion(containerItem.second);
            firstItem = false;
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMap(std::map<T1, T2, C, A> const &container,
                  std::function<std::string(T1 const &)> const &keyStringConversion,
                  std::function<std::string(T2 const &)> const &valueStringConversion,
                  std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n",
                  bool withNewline = true) {
        printMap(container, {}, keyStringConversion, valueStringConversion, keyValueSeparator, itemSeparator,
                 withNewline);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMapConvertKey(std::map<T1, T2, C, A> const &container,
                            std::function<bool(T1 const &, T2 const &)> const &elementFilter,
                            std::function<std::string(T1 const &)> const &keyStringConversion,
                            std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n",
                            bool withNewline = true) {
        bool firstItem = true;
        for (auto const &containerItem: container) {
            if (elementFilter && elementFilter(containerItem.first, containerItem.second)) {
                continue;
            }
            if (!firstItem) {
                std::cout << itemSeparator;
            }
            std::cout << keyStringConversion(containerItem.first) << keyValueSeparator << containerItem.second;
            firstItem = false;
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMapConvertKey(std::map<T1, T2, C, A> const &container,
                            std::function<std::string(T1 const &)> const &keyStringConversion,
                            std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n",
                            bool withNewline = true) {
        printMapConvertKey(container, {}, keyStringConversion, keyValueSeparator, itemSeparator, withNewline);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMapConvertValue(std::map<T1, T2, C, A> const &container,
                              std::function<bool(T1 const &, T2 const &)> const &elementFilter,
                              std::function<std::string(T2 const &)> const &valueStringConversion,
                              std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n",
                              bool withNewline = true) {
        bool firstItem = true;
        for (auto const &containerItem: container) {
            if (elementFilter && elementFilter(containerItem.first, containerItem.second)) {
                continue;
            }
            if (!firstItem) {
                std::cout << itemSeparator;
            }
            std::cout << containerItem.first << keyValueSeparator << valueStringConversion(containerItem.second);
            firstItem = false;
        }
        if (withNewline) {
            std::cout << std::endl;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void printMapConvertValue(std::map<T1, T2, C, A> const &container,
                              std::function<std::string(T2 const &)> const &valueStringConversion,
                              std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n",
                              bool withNewline = true) {
        printMapConvertValue(container, {}, valueStringConversion, keyValueSeparator, itemSeparator, withNewline);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::string printMapToString(
            std::map<T1, T2, C, A> const &container, std::function<bool(T1 const &, T2 const &)> const &elementFilter,
            std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n") {
        std::stringstream ss;
        bool firstItem = true;
        for (auto const &containerItem: container) {
            if (elementFilter && elementFilter(containerItem.first, containerItem.second)) {
                continue;
            }
            if (!firstItem) {
                ss << itemSeparator;
            }
            ss << containerItem.first << keyValueSeparator << containerItem.second;
            firstItem = false;
        }
        return ss.str();
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::string printMapToString(
            std::map<T1, T2, C, A> const &container, std::string const &keyValueSeparator = " -> ",
            std::string const &itemSeparator = "\n") {
        return printMapToString(container, {}, keyValueSeparator, itemSeparator);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::string printMapToString(
            std::map<T1, T2, C, A> const &container, std::function<bool(T1 const &, T2 const &)> const &elementFilter,
            std::function<std::string(T1 const &)> const &keyStringConversion,
            std::function<std::string(T2 const &)> const &valueStringConversion,
            std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n") {
        std::stringstream ss;
        bool firstItem = true;
        for (auto const &containerItem: container) {
            if (elementFilter && elementFilter(containerItem.first, containerItem.second)) {
                continue;
            }
            if (!firstItem) {
                ss << itemSeparator;
            }
            ss << keyStringConversion(containerItem.first) << keyValueSeparator
               << valueStringConversion(containerItem.second);
            firstItem = false;
        }
        return ss.str();
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::string printMapToString(
            std::map<T1, T2, C, A> const &container, std::function<std::string(T1 const &)> const &keyStringConversion,
            std::function<std::string(T2 const &)> const &valueStringConversion,
            std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n") {
        return printMapToString(container, {}, keyStringConversion, valueStringConversion, keyValueSeparator,
                                itemSeparator);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::string printMapToStringConvertKey(
            std::map<T1, T2, C, A> const &container, std::function<bool(T1 const &, T2 const &)> const &elementFilter,
            std::function<std::string(T1 const &)> const &keyStringConversion,
            std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n") {
        std::stringstream ss;
        bool firstItem = true;
        for (auto const &containerItem: container) {
            if (elementFilter && elementFilter(containerItem.first, containerItem.second)) {
                continue;
            }
            if (!firstItem) {
                ss << itemSeparator;
            }
            ss << keyStringConversion(containerItem.first) << keyValueSeparator << containerItem.second;
            firstItem = false;
        }
        return ss.str();
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::string printMapToStringConvertKey(
            std::map<T1, T2, C, A> const &container, std::function<std::string(T1 const &)> const &keyStringConversion,
            std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n") {
        return printMapToStringConvertKey(container, {}, keyStringConversion, keyValueSeparator, itemSeparator);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::string printMapToStringConvertValue(
            std::map<T1, T2, C, A> const &container, std::function<bool(T1 const &, T2 const &)> const &elementFilter,
            std::function<std::string(T2 const &)> const &valueStringConversion,
            std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n") {
        std::stringstream ss;
        bool firstItem = true;
        for (auto const &containerItem: container) {
            if (elementFilter && elementFilter(containerItem.first, containerItem.second)) {
                continue;
            }
            if (!firstItem) {
                ss << itemSeparator;
            }
            ss << containerItem.first << keyValueSeparator << valueStringConversion(containerItem.second);
            firstItem = false;
        }
        return ss.str();
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::string printMapToStringConvertValue(
            std::map<T1, T2, C, A> const &container,
            std::function<std::string(T2 const &)> const &valueStringConversion,
            std::string const &keyValueSeparator = " -> ", std::string const &itemSeparator = "\n") {
        return printMapToStringConvertValue(container, {}, valueStringConversion, keyValueSeparator, itemSeparator);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::vector<T1> getMapKeys(std::map<T1, T2, C, A> const &container) {
        std::vector<T1> keys(container.size());
        int i = 0;
        for (auto const &elem: container) {
            keys[i++] = elem.first;
        }
        return keys;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::vector<T2> getMapValues(std::map<T1, T2, C, A> const &container) {
        std::vector<T2> values(container.size());
        int i = 0;
        for (auto const &elem: container) {
            values[i++] = elem.second;
        }
        return values;
    }

    template<class T1, class T2>
    [[nodiscard]] std::map<T1, T2>
    createMapFromKeysAndValues(std::vector<T1> const &keys, std::vector<T2> const &values) {
        assert(keys.size() == values.size());
        std::map<T1, T2> res;
        for (int i = 0; i < keys.size(); i++) {
            res[keys[i]] = values[i];
        }
        return res;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void filterAndOperateOnMap(std::map<T1, T2, C, A> &container,
                               std::function<void(typename std::map<T1, T2, C, A>::citerator const &)> const &op,
                               std::function<bool(T1 const &, T2 const &)> const &predicate) {
        for (auto elem = container.cbegin(); elem != container.cend();) {
            op(elem);
            if (predicate(elem->first, elem->second)) {
                elem = container.erase(elem);
            } else {
                ++elem;
            }
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void filterAndOperateOnMap(std::map<T1, T2, C, A> &container,
                               std::function<void(typename std::map<T1, T2, C, A>::iterator const &)> const &op,
                               std::function<bool(T1 const &, T2 const &)> const &predicate) {
        for (auto elem = container.begin(); elem != container.end();) {
            op(elem);
            if (predicate(elem->first, elem->second)) {
                elem = container.erase(elem);
            } else {
                ++elem;
            }
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void filterMapBasedOnPredicate(std::map<T1, T2, C, A> &container,
                                   std::function<bool(T1 const &, T2 const &)> const &predicate) {
        for (auto elem = container.cbegin(); elem != container.cend();) {
            if (predicate(elem->first, elem->second)) {
                elem = container.erase(elem);
            } else {
                ++elem;
            }
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::map<T1, T2, C, A> getFilteredMapBasedOnPredicate(
            std::map<T1, T2, C, A> const &container, std::function<bool(T1 const &, T2 const &)> const &predicate) {
        std::map<T1, T2, C, A> result = container;
        filterMapBasedOnPredicate(result, predicate);
        return result;
    }

    template<class T, typename A = std::allocator<std::pair<const int, T>>>
    [[nodiscard]] std::map<int, T, std::less<int>, A> mapFromVector(std::vector<T> const &v) {
        std::map<int, T, std::less<int>, A> m;
        for (size_t i = 0; i < v.size(); i++) {
            m[i] = v[i];
        }
        return m;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::map<T1, T2, C, A> mapFromVector(
            std::vector<T2> const &v, std::function<T1(T2 const &)> const &keyOp) {
        std::map<T1, T2, C, A> m;
        for (size_t i = 0; i < v.size(); i++) {
            m[keyOp(v[i])] = v[i];
        }
        return m;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::map<T1, T2, C, A> mapFromVector(
            std::vector<T2> const &v, std::function<T1(T2 const &, size_t)> const &keyOp) {
        std::map<T1, T2, C, A> m;
        for (size_t i = 0; i < v.size(); i++) {
            m[keyOp(v[i], i)] = v[i];
        }
        return m;
    }

    template<class T, typename C = std::less<T>, typename A = std::allocator<std::pair<const T, bool>>>
    [[nodiscard]] std::map<T, bool, C, A> setMapKeysFromVectorElements(std::vector<T> const &v) {
        std::map<T, bool, C, A> m;
        for (size_t i = 0; i < v.size(); i++) {
            m[v[i]] = true;
        }
        return m;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] std::map<T1, T2, C, A> mapAppend(std::map<T1, T2, C, A> const &container,
                                                   std::map<T1, T2, C, A> const &valuesToBeAppended) {
        auto res = container;
        res.insert(valuesToBeAppended.begin(), valuesToBeAppended.end());
        return res;
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void mapAppendInPlace(std::map<T1, T2, C, A> &container, std::map<T1, T2, C, A> const &valuesToBeAppended) {
        container.insert(valuesToBeAppended.begin(), valuesToBeAppended.end());
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] T2 const &sampleFromMap(std::map<T1, T2, C, A> const &m) {
        RandomNumberGenerator<int> sampler(0, m.size() - 1);
        return sampleFromMap(m, sampler);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] T2 &sampleFromMap(std::map<T1, T2, C, A> &m) {
        RandomNumberGenerator<int> sampler(0, m.size() - 1);
        return sampleFromMap(m, sampler);
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] T2 const &sampleFromMap(std::map<T1, T2, C, A> const &m, RandomNumberGenerator<int> &sampler) {
        if (m.empty()) {
            throw std::runtime_error("Can not sample from an empty map!");
        }
        int sampledIndex = sampler.sample(), index = 0;
        if (sampledIndex >= m.size()) {
            throw std::runtime_error("Sampled index is greater than or equal to the map size!");
        }
        for (auto const &data: m) {
            if (index++ == sampledIndex) {
                return data.second;
            }
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    [[nodiscard]] T2 &sampleFromMap(std::map<T1, T2, C, A> &m, RandomNumberGenerator<int> &sampler) {
        if (m.empty()) {
            throw std::runtime_error("Can not sample from an empty map!");
        }
        int sampledIndex = sampler.sample(), index = 0;
        if (sampledIndex >= m.size()) {
            throw std::runtime_error("Sampled index is greater than or equal to the map size!");
        }
        for (auto &data: m) {
            if (index++ == sampledIndex) {
                return data.second;
            }
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void getSingleElement(std::map<T1, T2, C, A> const &m, T1 &key, T2 &value) {
        if (m.size() != 1) {
            throw std::runtime_error("Map does not have only one element (" + std::to_string(m.size()) + ")!");
        }
        for (auto const &elem: m) {
            key = elem.first;
            value = elem.second;
        }
    }

    template<class T1, class T2, typename C = std::less<T1>, typename A = std::allocator<std::pair<const T1, T2>>>
    void getSingleElement(std::map<T1, T2, C, A> &m, T1 *&key, T2 *&value) {
        if (m.size() != 1) {
            throw std::runtime_error("Map does not have only one element (" + std::to_string(m.size()) + ")!");
        }
        for (auto &elem: m) {
            key = &elem.first;
            value = &elem.second;
        }
    }
}
