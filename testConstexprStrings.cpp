//
// Created by Andrei on 13.08.25.
//

#include <AndreiUtils/classes/ConstexprString.hpp>

using namespace AndreiUtils;

// ----------------- Usage -----------------
constexpr auto combined = concatenate("hello", " ", "world", "!");
constexpr auto combined2 = concatenate("hello");

class Base {
public:
    static constexpr std::size_t name_size = totalLength<decltype("Base")>();

    static constexpr auto getName() {
        return concatenate("Base");
    }
};

class Derived : public Base {
public:
    static constexpr std::size_t name_size = totalLength<
        decltype("Derived"), decltype(" "), decltype("from "), decltype(Base::getName())>();

    static constexpr auto getName() {
        return concatenate("Derived", " ", "from ", Base::getName());
    }
};

template<class... Types>
class ListOfTypes {
public:
    static int const size = sizeof...(Types);

    template<bool Negate = false>
    static constexpr auto typesToString() {
        if constexpr (size == 0) {
            return ConstexprString<0>{}; // empty
        } else {
            return joinNames<Negate, Types...>();
        }
    }

protected:
    template<bool Negate, typename First, typename... Rest>
    static constexpr auto joinNames() {
        if constexpr (sizeof...(Rest) == 0) {
            if constexpr (Negate) {
                return concatenate("!", First::getName());
            } else {
                return First::getName();
            }
        } else {
            if constexpr (Negate) {
                return concatenate("!", First::getName(), ", ", joinNames<Negate, Rest...>());
            } else {
                return concatenate(First::getName(), ", ", joinNames<Negate, Rest...>());
            }
        }
    }
};

template<typename AcceptTypes = ListOfTypes<>, typename RejectTypes = ListOfTypes<>>
class Instance {
public:
    static constexpr auto getName() {
        if constexpr (RejectTypes::size == 0) {
            return concatenate("Instance<", AcceptTypes::template typesToString<>(), ">");
        } else {
            return concatenate(
                    "Instance<", AcceptTypes::template typesToString<>(), ", ",
                    RejectTypes::template typesToString<true>(), ">");
        }
    }
};

#include <iostream>

int main() {
    static_assert(HasDataAndSize<ConstexprString<4>>);

    static_assert(concatenate("").size == 0, "Error");
    static_assert(std::is_array_v<const char [5]>, "Error");
    static_assert(std::is_array_v<std::remove_reference_t<const char (&)[5]>>, "Error");
    static_assert(
            std::is_same_v<char, std::remove_extent_t<std::remove_const_t<std::remove_reference_t<const char (&)[5]>>>>,
            "Error");
    static_assert(totalLength<decltype("Base")>() == 4, "Error");
    // std::is_same_v<char, std::remove_extent_t<T>>

    std::cout << combined.view() << "\n"; // prints "hello world!"
    auto const x = concatenate("123", IntToConstexprString<1234>{}, "21345");
    std::cout << x.view() << "\n"; // prints "123123421345"
    std::cout << x.c_str() << "\n"; // prints "123123421345"
    std::cout << ListOfTypes<Derived, Base, Base, Derived>::typesToString<>().view() << std::endl;
    std::cout << ListOfTypes<Derived, Base, Base, Derived>::typesToString<true>().view() << std::endl;
    std::cout << Instance<ListOfTypes<Derived, Base, Base, Derived>>::getName().view() << std::endl;
    std::cout << Instance<ListOfTypes<Derived, Base, Base, Derived>, ListOfTypes<Derived>>::getName().view() << std::endl;
    std::cout << Instance<ListOfTypes<Base, Base>, ListOfTypes<Derived>>::getName().view() << std::endl;

    std::string s{combined};
    std::string s1 = combined;
    std::cout << s << std::endl;
    std::cout << s1 << std::endl;
    std::cout << static_cast<std::string>(combined) << std::endl;

    static_assert(HasDataAndSize<IntToConstexprString<42>>, "Error");

    std::cout << ("Hello " + x) << std::endl;
    std::cout << (x + "Hello ") << std::endl;
    std::cout << x + x << std::endl;

    std::cout << AndreiUtils::concatenate(IntToConstexprString<1234>{}, BoolToConstexprString<false>{}, "Hello World!").view() << std::endl;
    std::cout << x + IntToConstexprString<1234>{} << std::endl;
    std::cout << IntToConstexprString<1234>{} + x << std::endl;
    std::cout << IntToConstexprString<1234>{} + IntToConstexprString<2345>{} << std::endl;

    std::cout << x + BoolToConstexprString<false>{} << std::endl;
    std::cout << BoolToConstexprString<false>{} + x << std::endl;
    std::cout << BoolToConstexprString<false>{} + BoolToConstexprString<true>{} << std::endl;

    std::cout << IntToConstexprString<12>{} + BoolToConstexprString<true>{} << std::endl;
    std::cout << BoolToConstexprString<false>{} + IntToConstexprString<42>{} << std::endl;
    std::cout << "Hello " + IntToConstexprString<1234>{} << std::endl;
    std::cout << IntToConstexprString<1234>{} + " World" << std::endl;
    std::cout << BoolToConstexprString<false>{} + "World" << std::endl;

    std::cout << (IntToConstexprString<123>{} == IntToConstexprString<123>{}) << std::endl;
    std::cout << (IntToConstexprString<123>{} == IntToConstexprString<124>{}) << std::endl;
    std::cout << (BoolToConstexprString<false>{} == BoolToConstexprString<false>{}) << std::endl;
    std::cout << (BoolToConstexprString<true>{} == BoolToConstexprString<true>{}) << std::endl;
    std::cout << (BoolToConstexprString<true>{} == BoolToConstexprString<false>{}) << std::endl;
    std::cout << (ConstexprString<4>{"1234"} == ConstexprString<4>{"1234"}) << std::endl;
    std::cout << (ConstexprString<4>{"1234"} == ConstexprString<4>{"1235"}) << std::endl;
    std::cout << (ConstexprString<4>{"1234"} == ConstexprString<5>{"1235"}) << std::endl;
    std::cout << (ConstexprString<5>{"1234"} == ConstexprString<5>{"1234\0"}) << std::endl;
    std::cout << (x == x) << std::endl;
    std::cout << (x != x) << std::endl;

    std::cout << ("1234" == x) << std::endl;
    std::cout << ("1234" != x) << std::endl;
    std::cout << (std::string("1234") != x) << std::endl;
    std::cout << (std::string("1234") == x) << std::endl;
    std::cout << (std::string("1234") == std::string("x")) << std::endl;
    std::cout << (std::string("1234") != std::string("x")) << std::endl;
}
