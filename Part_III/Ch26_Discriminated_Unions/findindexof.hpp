#pragma once

#include <type_traits>
#include "typelist/typelist.hpp"
#include "typelist/is_empty.hpp"
#include "typelist/typelist_algorithm.hpp"


template<typename List, typename T, unsigned N = 0,
         bool = is_empty_v<List>>
struct find_index_of;

template<typename List, typename T, unsigned N>
struct find_index_of<List, T, N, false>
    : public if_then_else_t<std::is_same_v<front_t<List>, T>,
                            std::integral_constant<unsigned, N>,
                            find_index_of<pop_front_t<List>, T, N+1>>
{
};

template<typename List, typename T, unsigned N>
struct find_index_of<List, T, N, true>
{
};

template<typename List, typename T, unsigned N>
constexpr inline auto find_index_of_v = find_index_of<List,T,N>::value;
