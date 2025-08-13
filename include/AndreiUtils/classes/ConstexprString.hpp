//
// Created by Andrei on 13.08.25.
//

#pragma once

#include <array>
#include <string_view>
#include <string>
#include <cstddef>

namespace AndreiUtils {
    // Struct to hold compile-time string and provide string_view
    template<std::size_t N>
    struct ConstexprString {
        static constexpr std::size_t size = N;

        std::array<char, N + 1> data{};  // +1 for null terminator

        constexpr ConstexprString() = default;

        // Implicit conversion to std::string at runtime
        operator std::string() const {
            return std::string(this->view());
        }

        template<typename T>
        constexpr void appendToData(T const &arg, std::size_t &pos) {
            if constexpr (std::is_array_v<T> && std::is_same_v<char, std::remove_extent_t<T>>) {
                for (std::size_t i = 0; i < std::extent_v<T> - 1; ++i)
                    this->data[pos++] = arg[i];
            } else if constexpr (requires { T::data; T::size; }) {
                for (std::size_t i = 0; i < T::size; ++i)
                    this->data[pos++] = arg.data[i];
            } else {
                static_assert(sizeof(T) == 0, "Unsupported type for concatenate");
            }
        }

        template<typename... Args>
        explicit constexpr ConstexprString(Args const &... args) {
            std::size_t pos = 0;
            (appendToData(args, pos), ...);
            this->data[N] = '\0';
        }

        constexpr char const *c_str() const { return this->data.data(); }

        constexpr std::string_view view() const { return std::string_view(data.data(), N); }
    };

    template<typename T>
    constexpr std::size_t argLength() {
        if constexpr (std::is_array_v<std::remove_reference_t<T>> &&
                      std::is_same_v<char, std::remove_extent_t<std::remove_const_t<std::remove_reference_t<T>>>>) {
            return std::extent_v<std::remove_reference_t<T>> - 1;
        } else if constexpr (requires { T::size; }) {
            return T::size;
        } else {
            static_assert(sizeof(T) == 0, "Unsupported type for concatenate");
            return 0;
        }
    }

    template <auto N>
    struct IntToConstexprString {
    private:
        static constexpr std::size_t num_digits() {
            auto n = N;
            std::size_t digits = (n == 0) ? 1 : 0;
            if (n < 0) {
                ++digits; // for '-'
                n = -n;
            }
            while (n > 0) {
                n /= 10;
                ++digits;
            }
            return digits;
        }

    public:
        static constexpr std::size_t size = num_digits();
        std::array<char, size + 1> data{}; // +1 for '\0'

        // Constructor: fills the array at compile time
        constexpr IntToConstexprString() : data{} {
            auto n = N;
            std::size_t pos = size;
            this->data[pos] = '\0';

            bool neg = false;
            if (n < 0) {
                neg = true;
                n = -n;
            }

            do {
                this->data[--pos] = static_cast<char>('0' + (n % 10));
                n /= 10;
            } while (n > 0);

            if (neg) {
                this->data[0] = '-';
            }
        }

        // Allow implicit conversion to const char*
        constexpr operator const char*() const {
            return this->data.data();
        }
    };

    template<typename... Args>
    constexpr std::size_t totalLength() { return (0 + ... + argLength<Args>()); }

    // Function to concatenate compile-time strings
    template<typename... Args>
    constexpr auto concatenate(Args const &... args) { return ConstexprString<totalLength<Args...>()>{args...}; }
}
