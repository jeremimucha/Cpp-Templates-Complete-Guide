#pragma once
#include "typelist.hpp"


// nth_element - get the nth element of a typelist

// recursive case:
template<typename List, unsigned N>
struct nth_element : nth_element<pop_front_t<List>, N-1> { };

// basic case
template<typename List>
struct nth_element<List, 0> : front<List> { };

template<typename List, unsigned N>
using nth_element_t = typename nth_element<List,N>::type;


// unit tests
namespace unit_test_nth_element
{
    using lst_n3 = typelist<int, double, char>;
    static_assert(std::is_same_v<nth_element_t<lst_n3,0>,int>);
    static_assert(std::is_same_v<nth_element_t<lst_n3,1>,double>);
    static_assert(std::is_same_v<nth_element_t<lst_n3,2>,char>);
    // static_assert(std::is_same_v<nth_element_t<lst_n3,100>,void>);   // compile error - GOOD
} // namespace unit_test_nth_element
