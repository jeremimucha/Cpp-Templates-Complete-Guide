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
/* --------------------------------------------------------------------------------------------- */

// pop front
/* --------------------------------------------------------------------------------------------- */
template<typename T, typename... Types>
Tuple<Types...> popFront(Tuple<T,Types...> const& tuple)
{
    return tuple.getTail();
}
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
    return pushBack(reverse(t.getTail()), t.getHead());
}
/* --------------------------------------------------------------------------------------------- */

// pop back
/* --------------------------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------------------------- */