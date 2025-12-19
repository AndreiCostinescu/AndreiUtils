//
// Created by Andrei on 13.08.25.
//

#pragma once

#include <algorithm>
#include <AndreiUtils/traits/stringify.hpp>
#include <array>
#include <string_view>
#include <string>
#include <cstddef>
#include <concepts>
#include <type_traits>

namespace AndreiUtils {
    template<typename T>
    concept HasDataAndSize =
            // 1) Require that T::size is a constant expression convertible to std::size_t
            requires
            {
                typename std::integral_constant<std::size_t, std::remove_cvref_t<T>::size>;
            }
            // 2) Require that there's a non-static member `data` whose type is std::array<char, T::size>
            && requires(std::remove_cvref_t<T> &u)
            {
                requires std::same_as<decltype((u.data)), std::array<char, std::remove_cvref_t<T>::size + 1> &>;
            }
            // 3) Allow for const types
            && requires(std::remove_cvref_t<T> const &cu)
            {
                requires std::same_as<decltype((cu.data)), std::array<char, std::remove_cvref_t<T>::size + 1> const &>;
            }
            && !std::same_as<std::remove_cvref_t<T>, std::string>
            && !std::same_as<std::remove_cvref_t<T>, std::string_view>;

    // Struct to hold compile-time string and provide string_view
    template<std::size_t N>
    struct ConstexprString {
        static constexpr std::size_t size = N;
        std::array<char, size + 1> data{};  // +1 for null terminator

        constexpr ConstexprString() = default;

        template<typename... Args>
        explicit constexpr ConstexprString(Args const &... args) {
            std::size_t pos = 0;
            (appendToData(args, pos), ...);
            this->data[N] = '\0';
        }

        // don't make this explicit
        operator std::string() const {
            return std::string(this->view());
        }

        [[nodiscard]] constexpr std::string_view view() const { return std::string_view(this->data.data(), size); }

        [[nodiscard]] constexpr char const *c_str() const { return this->data.data(); }

    protected:
        template<typename T>
        constexpr void appendToData(T const &arg, std::size_t &pos) {
            if constexpr (std::is_array_v<T> && std::is_same_v<char, std::remove_extent_t<T>>) {
                for (std::size_t i = 0; i < std::extent_v<T> - 1; ++i)
                    this->data[pos++] = arg[i];
            } else if constexpr (HasDataAndSize<T>) {
                for (std::size_t i = 0; i < T::size; ++i)
                    this->data[pos++] = arg.data[i];
            } else {
                static_assert(sizeof(T) == 0, "Unsupported type for concatenate");
            }
        }
    };

    template<auto N>
    struct IntToConstexprString {
    protected:
        static constexpr std::size_t numDigits() {
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
        static constexpr std::size_t size = numDigits();
        std::array<char, size + 1> data{}; // +1 for '\0'

        // Constructor: fills the array at compile time
        constexpr IntToConstexprString() {
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

        // don't make this explicit
        operator std::string() const {
            return std::string(this->view());
        }

        [[nodiscard]] constexpr std::string_view view() const { return std::string_view(this->data.data(), size); }

        [[nodiscard]] constexpr char const *c_str() const { return this->data.data(); }
    };

    template<bool B>
    struct BoolToConstexprString {
    protected:
        static constexpr std::size_t numChars() {
            if constexpr (B) {
                return 4;
            } else {
                return 5;
            }
        }

    public:
        static constexpr std::size_t size = numChars();
        std::array<char, size + 1> data{}; // +1 for '\0'

        // Constructor: fills the array at compile time
        constexpr BoolToConstexprString() {
            std::size_t pos = size;
            this->data[pos] = '\0';
            if constexpr (B) {
                this->data[0] = 't';
                this->data[1] = 'r';
                this->data[2] = 'u';
                this->data[3] = 'e';
            } else {
                this->data[0] = 'f';
                this->data[1] = 'a';
                this->data[2] = 'l';
                this->data[3] = 's';
                this->data[4] = 'e';
            }
        }

        // don't make this explicit
        operator std::string() const {
            return std::string(this->view());
        }

        [[nodiscard]] constexpr std::string_view view() const { return std::string_view(this->data.data(), size); }

        [[nodiscard]] constexpr char const *c_str() const { return this->data.data(); }
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

    template<typename... Args>
    constexpr std::size_t totalLength() { return (0 + ... + argLength<Args>()); }

    // Function to concatenate compile-time strings
    template<typename... Args>
    constexpr auto concatenate(Args const &... args) { return ConstexprString<totalLength<Args...>()>{args...}; }

    // std::string + ConstexprString-like
    template<HasDataAndSize T>
    std::string operator+(std::string const &lhs, T const &rhs) {
        return lhs + std::string(rhs);
    }

    // ConstexprString-like + std::string
    template<HasDataAndSize T>
    std::string operator+(T const &lhs, std::string const &rhs) {
        return std::string(lhs) + rhs;
    }

    // ConstexprString-like + ConstexprString-like
    template<HasDataAndSize T1, HasDataAndSize T2>
    std::string operator+(T1 const &lhs, T2 const &rhs) {
        return std::string{concatenate(lhs, rhs)};
    }

    template<std::size_t N>
    struct stringify<ConstexprString<N>> {
        static std::string to_string(ConstexprString<N> const &datum) {
            return std::string(datum);
        }
    };

    template<auto N>
    struct stringify<IntToConstexprString<N>> {
        static std::string to_string(IntToConstexprString<N> const &datum) {
            return std::string(datum);
        }
    };

    template<bool B>
    struct stringify<BoolToConstexprString<B>> {
        static std::string to_string(BoolToConstexprString<B> const &datum) {
            return std::string(datum);
        }
    };

    template<HasDataAndSize T>
    bool operator==(std::string const &lhs, T const &rhs) {
        return lhs == std::string{rhs};
    }

    template<HasDataAndSize T>
    bool operator==(T const &lhs, std::string const &rhs) {
        return rhs == std::string{lhs};
    }

    template<HasDataAndSize T>
    bool operator!=(std::string const &lhs, T const &rhs) {
        return !operator==(lhs, rhs);
    }

    template<HasDataAndSize T>
    bool operator!=(T const &lhs, std::string const &rhs) {
        return !operator==(lhs, rhs);
    }

    template<HasDataAndSize T1, HasDataAndSize T2>
    constexpr bool operator==(T1 const &lhs, T2 const &rhs) {
        return lhs.size == rhs.size && std::equal(lhs.data.begin(), lhs.data.end(), rhs.data.begin());
    }

    template<HasDataAndSize T1, HasDataAndSize T2>
    constexpr bool operator!=(T1 const &lhs, T2 const &rhs) {
        return !operator==(lhs, rhs);
    }
}
