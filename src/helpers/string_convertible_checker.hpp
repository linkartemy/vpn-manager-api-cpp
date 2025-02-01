#pragma once

#include <sstream>
#include <string>
#include <type_traits>

template <typename T, typename = void>
struct is_string_convertible : std::false_type {};

template <typename T>
struct is_string_convertible<
    T,
    std::void_t<
        std::enable_if_t<std::is_convertible_v<
            decltype(std::to_string(std::declval<T>())), std::string>>,
        std::enable_if_t<std::is_convertible_v<
            decltype(std::declval<std::stringstream&>() << std::declval<T>()),
            std::ostream&>>>> : std::true_type {};

template <typename T>
inline constexpr bool is_string_convertible_v = is_string_convertible<T>::value;
