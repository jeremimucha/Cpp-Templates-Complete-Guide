#pragma once

#include "typelist/typelist.hpp"
#include "typelist/value_lists.hpp"
#include "typelist/nth_element.hpp"
#include "makeindexlist.hpp"
#include "basic_tuple.hpp"

// The reverseTuple implementation using pop_front an push_back needlesly copies every
// tuple element multiple times at runtime. This can be avoided by indexing the tuple, instead of
// walking it. valuelist (std::integer_sequence) is used to construct a list of indices
// at compiletime. The index list is then expanded to provide individual indices to get.

template<typename... Elements, unsigned... Indices>
auto reverseTupleImpl(Tuple<Elements...> const& t, valuelist<unsigned, Indices...>)
{
    return makeTuple(get<Indices>(t)...);
}

template<typename... Elements, unsigned... Indices>
auto reverseTuple(Tuple<Elements...> const& t)
{
    return reverseTupleImpl(t, reverse_t<make_index_list<sizeof...(Elements)>>{});
}


namespace reverseTuple_unit_test
{
    Tuple<int,double,char,float> tt{1,2.2,'a',4.4f};
    auto tt2 = reverseTuple(tt);
    static_assert(std::is_same_v<decltype(tt2), Tuple<float,char,double,int>>);
} // reverseTuple_unit_test


// Select - return a tuple created by indexing another tuple with the given indexlist
/* --------------------------------------------------------------------------------------------- */
template<typename... Elements, unsigned... Is>
auto select(Tuple<Elements...> const& t, valuelist<unsigned,Is...>)
{
    return makeTuple(get<Is>(t)...);
}
/* --------------------------------------------------------------------------------------------- */


// splat - return a tuple with element at index I replicated N times
/* --------------------------------------------------------------------------------------------- */
template<unsigned I, unsigned N, typename IndexList = valuelist<unsigned>>
struct replicated_index_list_impl;

template<unsigned I, unsigned N, unsigned... Indices>
struct replicated_index_list_impl<I, N, valuelist<unsigned, Indices...>>
    : replicated_index_list_impl<I, N-1, valuelist<unsigned, Indices..., I>>
{};

template<unsigned I, unsigned... Indices>
struct replicated_index_list_impl<I, 0, valuelist<unsigned, Indices...>>
{
    using type = valuelist<unsigned, Indices...>;
};

template<unsigned I, unsigned N>
using replicated_index_list = typename replicated_index_list_impl<I,N>::type;

template<unsigned I, unsigned N, typename... Elements>
auto splat(Tuple<Elements...> const& t)
{
    return select(t, replicated_index_list<I,N>{});
}
/* --------------------------------------------------------------------------------------------- */

// TupleSort - sort a tuple via indexing
/* --------------------------------------------------------------------------------------------- */
// metafunction wrapper that compares the elements in a tuple
template<typename List, template<typename T, typename U> class F>
struct MetafunOfNthElement
{
    template<typename T, typename U> struct apply;

    template<unsigned N, unsigned M>
    struct apply<ct_value<unsigned,M>, ct_value<unsigned,N>>
        : F<nth_element_t<List,M>, nth_element_t<List,N>> { };
};
// sort a tuple based on comparing the element types:
template<template<typename T, typename U>class Compare,
         typename... Elements>
auto sort(Tuple<Elements...> const& tup)
{
    return select(tup,
                  insertion_sort_t<make_index_list<sizeof...(Elements)>,
                                   MetafunOfNthElement<
                                        Tuple<Elements...>,
                                        Compare>::template apply>{});
}

/* --------------------------------------------------------------------------------------------- */


// apply - expanding a tuple
/* --------------------------------------------------------------------------------------------- */
template<typename> struct tupleSize_impl;
template<template<typename...>class T, typename... Ts>
struct tupleSize_impl<T<Ts...>> { static constexpr inline auto value = sizeof...(Ts); };
template<typename T>
constexpr inline auto tupleSize{tupleSize_impl<T>::value};

template<typename F, typename Tup, unsigned... Indices>
constexpr auto apply_impl(F&& f, Tup&& tup, valuelist<unsigned, Indices...>)
{
    return std::forward<F>(f)(get<Indices>(std::forward<Tup>(tup))...);
}

template<typename F, typename Tup>
constexpr auto apply(F&& f, Tup&& tup)
{
    using Indices = make_index_list<tupleSize<Tup>>;
    return apply_impl(std::forward<F>(f), tup, Indices{});
}

static_assert(tupleSize<Tuple<int,double,char>> == 3);
/* --------------------------------------------------------------------------------------------- */