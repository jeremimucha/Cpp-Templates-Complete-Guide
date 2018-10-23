#pragma once

#include "typelist/value_lists.hpp"
#include <type_traits>

template<unsigned N, typename Result = valuelist<unsigned>>
struct make_index_list_impl : make_index_list_impl<N-1, push_front_t<Result, ct_value<unsigned,N-1>>>
{
};

template<typename Result>
struct make_index_list_impl<0, Result>
{
    using type = Result;
};

template<unsigned N>
using make_index_list = typename make_index_list_impl<N>::type;


namespace makeindexlist_unittest
{
    using ut = make_index_list<5>;
    static_assert(std::is_same_v<ut, valuelist<unsigned,0,1,2,3,4>>);
} // makeindexlist_unittest
