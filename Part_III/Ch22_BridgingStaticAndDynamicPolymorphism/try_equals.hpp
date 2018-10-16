#pragma once

#include <stdexcept>

// type trait to check if a type is equality comparable
template<typename...> using void_t = void;

template<typename T>
using eq_expr = decltype(std::declval<T const&>() == std::declval<T const&>());

template<typename T>
using neg_eq_expr = decltype(!(std::declval<T const&>() == std::declval<T const&>()));

template<typename T, typename = void>
struct is_equality_comparable : std::false_type { };

// assume that a type is equality comparable only if the comparison result is convertible to bool
// and if the comparison result can be negated and is convertible to bool
template<typename T>
struct is_equality_comparable<T, void_t<eq_expr<T>, neg_eq_expr<T>>>
    : std::conjunction<
        std::is_convertible<eq_expr<T>, bool>,
        std::is_convertible<neg_eq_expr<T>, bool>> { };

template<typename T>
constexpr auto is_equality_comparable_v = is_equality_comparable<T>::value;


template<typename T, bool eq_comp = is_equality_comparable_v<T>>
struct try_equals {
    static bool equals(T const& lhs, T const& rhs) noexcept {
        return lhs == rhs;
    }
};

struct not_equality_comparable : public std::exception {};

template<typename T>
struct try_equals<T, false>
{
    static bool equals(T const& lhs, T const& rhs) {
        throw not_equality_comparable{};
    }
};
