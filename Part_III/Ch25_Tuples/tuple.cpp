#include <iostream>
#include "basic_tuple.hpp"
#include "tuple_print.hpp"
#include "tuple_comparison.hpp"
#include "tuple_algorithms.hpp"
#include "makeindexlist.hpp"
#include "indexing_algorithms.hpp"

#include <string>

template<typename T, typename U>
struct smaller_than {
    static constexpr bool value = sizeof(T) < sizeof(U);
};

void testTupleSort()
{
    auto t1 = makeTuple(17LL, std::string{"hello"}, 'c', 42, 7.7);
    std::cout << t1 << '\n';
    auto t2 = sort<smaller_than>(t1);
    std::cout << "sorted by size: " << t2 << '\n';
}


int main()
{
    testTupleSort();
}

template<typename> struct largest_type_impl;

template<template<typename...>class List, typename T, typename... Ts>
struct largest_type_impl<List<T,Ts...>>
    : std::conditional<sizeof(T) > sizeof(largest_type_impl<List<Ts...>),
                       T,
                       largest_type_impl<List<Ts...>>
                       >
{
};

template<template<typename...>class List, typename T>
struct largest_type_impl<List<T>> { using type = T; };
template<template<typename...>class List>
struct largest_type_impl<List<>> { using type = char; };

template<typename T>
struct largest_type : largest_type_impl<T> {};
template<typename T>
using largest_type_t = typename largest_type_impl<T>::type;
