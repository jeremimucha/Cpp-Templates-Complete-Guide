#pragma once

#include <type_traits>

// or define our ownn...
/* 
template<typename T, T val>
struct integral_constant {
    using type = integral_constant;
    using value_type = T;
    static inline constexpr value = val;
    constexpr value_type operator()() const noexcept { return value; }
    constexpr operator value_type() const noexcept { return value; }
};

template<bool B>
struct bool_constant : integral_constant<bool, B> { };

using false_type = bool_constant<false>;
using true_type = bool_constant<true>;
*/

template<typename T1, typename T2>
struct issame : std::false_type { };

template<typename T>
struct issame<T,T> : std::true_type { };

template<typename T1, typename T2>
using issame_t = typename issame<T1,T2>::type;

template<typename T1, typename T2>
static inline constexpr auto issame_v = issame<T1,T2>::value;
