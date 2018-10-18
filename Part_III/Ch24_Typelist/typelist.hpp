#pragma once
#include <type_traits>
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename... Ts>
struct typelist { };
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// last element of the list
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename> struct back_impl;

// generalize for any type holding a parameter pack
template<template<class> class C, typename T>
struct back_impl<C<T>> { using type = T; };    // the only element is the last element

template<template<class...> class C, typename Head, typename... Tail>
struct back_impl<C<Head,Tail...>> { using type = typename back_impl<C<Tail...>>::type; };


template<typename List>
struct back : back_impl<List> { };

template<typename List>
using back_t = typename back_impl<List>::type;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// first element of a list
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename> struct front_impl;

template<template<class...> class C, typename T, typename... Ts>
struct front_impl<C<T, Ts...>> { using type = T; };

// public interface

template<typename List>
struct front : front_impl<List> { };


template<typename List>
using front_t = typename front_impl<List>::type;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// Adding elements to a list
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// push front
template<typename L, typename... Ts> struct push_front_impl;

template<template<class...> class C, typename... Us, typename... Ts>
struct push_front_impl<C<Ts...>, Us...> {
    using type = C<Us..., Ts...>;
};

template<typename List, typename... Ts>
struct push_front : push_front_impl<List, Ts...> { };

template<typename List, typename... Ts>
using push_front_t = typename push_front_impl<List,Ts...>::type;

namespace a
{
    template<typename List, typename T>
    struct push_front : push_front_impl<List, T> { };

    template<typename List, typename T>
    using push_front_t = typename push_front_impl<List,T>::type;
}

// push_back
template<typename L, typename... Ts> struct push_back_impl;

template<template<class...> class C, typename... Us, typename... Ts>
struct push_back_impl<C<Ts...>, Us...> {
    using type = C<Ts..., Us...>;
};

template<typename List, typename... Ts>
struct push_back : push_back_impl<List, Ts...> { };

template<typename List, typename... Ts>
using push_back_t = typename push_back_impl<List, Ts...>::type;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// Removing elements from the list
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename L> struct pop_front_impl;

template<template<class...> class C, typename T, typename... Ts>
struct pop_front_impl<C<T,Ts...>> {
    using type = C<Ts...>;
};

template<template<typename...>class C>
struct pop_front_impl<C<>> {
    using type = C<>;
};

template<typename List>
struct pop_front : pop_front_impl<List> { };

template<typename List>
using pop_front_t = typename pop_front_impl<List>::type;

// TODO: figure out how to pop multiple elements at once
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// capacity
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename L, typename... Ts> struct count;

template<template<class...> class C, typename... Ts>
struct count<C<Ts...>> { static constexpr inline auto value = sizeof...(Ts); };

template<typename List>
static constexpr inline auto count_v = count<List>::value;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// Check if a type list contains the given type
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename C, typename T> struct contains_impl;

template<template<class...> class C, typename T, typename... Us>
struct contains_impl<C<Us...>, T> : std::disjunction<std::is_same<T, Us>...> { };

template<typename C, typename T>
struct contains_t : contains_impl<C,T> { };


template<typename C, typename T>
static constexpr inline auto contains_v{contains_impl<C,T>::value};
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

