#pragma once

#include "basic_tuple.hpp"
#include "typelist/typelist.hpp"
#include "typelist/typelist_algorithm.hpp"

// push front
/* --------------------------------------------------------------------------------------------- */
template<typename... Types, typename V>
Tuple<V,Types...> pushFront(Tuple<Types...> const& tuple, V const& value)
{
    return Tuple<V,Types...>{value, tuple};
}

namespace pushFront_unittest
{
    Tuple<int, double> tt{1,2.2};
    auto tt2 = pushFront(tt, 42.1f);
    static_assert(std::is_same_v<decltype(tt2), Tuple<float,int,double>>);
} // pushFront_unittest

/* --------------------------------------------------------------------------------------------- */

// push back
/* --------------------------------------------------------------------------------------------- */
// template<typename T, typename... Ts> struct push_back_impl;
// template<template<typename...>class T, typename... Ts, typename... Us>
// struct push_back_impl<T<Us...>, Ts...> { using type = T<Us...,Ts...>; };

// template<typename T, typename... Ts>
// struct push_back : push_back_impl<T,Ts...> { };
// template<typename T, typename... Ts>
// using push_back_t = typename push_back_impl<T,Ts...>::type;

// basis case
template<typename V>
Tuple<V> pushBack(Tuple<> const&, V const& value)
{
    return Tuple<V>{value};
}

// recursive case
template<typename Head, typename... Tail, typename V>
push_back_t<Tuple<Head,Tail...>,V> pushBack(Tuple<Head,Tail...> const& tuple, V const& value)
{
    return Tuple<Head, Tail..., V>(tuple.getHead(), pushBack(tuple.getTail(), value));
}

namespace pushBack_unittest
{
    Tuple<int, double> tt{1,2.2};
    auto tt2 = pushBack(tt, 42.1f);
    static_assert(std::is_same_v<decltype(tt2), Tuple<int,double,float>>);
} // pushBack_unittest

/* --------------------------------------------------------------------------------------------- */

// pop front
/* --------------------------------------------------------------------------------------------- */
template<typename T, typename... Types>
Tuple<Types...> popFront(Tuple<T,Types...> const& tuple)
{
    return tuple.getTail();
}

namespace popFront_unittest
{
    Tuple<int,double> tt{1,2.2};
    auto tt2 = popFront(tt);
    static_assert(std::is_same_v<decltype(tt2), Tuple<double>>);
} // popFront_unittest

/* --------------------------------------------------------------------------------------------- */

// reverse
/* --------------------------------------------------------------------------------------------- */
Tuple<> reverseTuple(Tuple<> const& t)
{
    return t;
}

template<typename Head, typename... Tail>
reverse_t<Tuple<Head,Tail...>> reverseTuple(Tuple<Head, Tail...> const& t)
{
    return pushBack(reverseTuple(t.getTail()), t.getHead());
}

namespace reverseTuple_unittest
{
    Tuple<int,double,char> tt{1,2.2,'a'};
    auto tt2 = reverseTuple(tt);
    static_assert(std::is_same_v<decltype(tt2), Tuple<char,double,int>>);
} // reverseTuple_unittest

/* --------------------------------------------------------------------------------------------- */

// pop back
/* --------------------------------------------------------------------------------------------- */
template<typename... Types>
pop_back_t<Tuple<Types...>> popBack(Tuple<Types...> const& t)
{
    return reverseTuple(popFront(reverseTuple(t)));
}

namespace popBack_unittest
{
    Tuple<int,double> tt{1,2.2};
    auto tt2 = popBack(tt);
    static_assert(std::is_same_v<decltype(tt2), Tuple<int>>);
} // popBack_unittest


namespace popBack_alt_impl
{

template<typename Tup>
struct popBackImpl;

template<template<typename...>class Tup, typename T, typename... Ts>
struct popBackImpl<Tup<T,Ts...>> {
    static pop_back_t<Tup<T,Ts...>> apply(Tup<T,Ts...> const& tup)
    {
        return pushFront(popBackImpl<Tup<Ts...>>::apply(tup.getTail()), tup.getHead());
    }
};

template<template<typename...>class Tup, typename T>
struct popBackImpl<Tup<T>> {
    static Tup<> apply(Tup<T> const& /*tup*/) { return Tup<>{}; }
};

template<typename... Types>
pop_back_t<Tuple<Types...>> popBack(Tuple<Types...> const& tup)
{
    return popBackImpl<Tuple<Types...>>::apply(tup);
}

    Tuple<int,double> tt{1,2.2};
    auto tt2 = popBack_alt_impl::popBack(tt);
    static_assert(std::is_same_v<decltype(tt2), Tuple<int>>);

} // popBack_alt_impl

/* --------------------------------------------------------------------------------------------- */
