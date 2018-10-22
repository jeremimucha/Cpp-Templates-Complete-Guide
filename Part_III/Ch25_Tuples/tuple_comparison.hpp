#pragma once

#include "basic_tuple.hpp"


// Since Tuples are structural types that contain other values it suffices to compare
// the elements of a tuple to deremine equality of two tuples.

// basis case
bool operator==(Tuple<> const&, Tuple<> const&)
{
    // empty tuples are always equivalent
    return true;
}

// recursive case
template<typename Head1, typename... Tail1,
         typename Head2, typename... Tail2,
         typename = enable_if_t<sizeof...(Tail1)==sizeof...(Tail2)>>
bool operator==(Tuple<Head1, Tail1...> const& lhs,
                Tuple<Head2, Tail2...> const& rhs)
{
    return lhs.getHead() == rhs.getHead() &&
           lhs.getTail() == rhs.getTail();
}

// !=
/* --------------------------------------------------------------------------------------------- */
bool operator!=(Tuple<> const&, Tuple<> const&)
{
    // empty tuples are always equivalent
    return false;
}

// recursive case
template<typename Head1, typename... Tail1,
         typename Head2, typename... Tail2,
         typename = enable_if_t<sizeof...(Tail1)==sizeof...(Tail2)>>
bool operator!=(Tuple<Head1, Tail1...> const& lhs,
                Tuple<Head2, Tail2...> const& rhs)
{
    return lhs.getHead() != rhs.getHead() ||
           lhs.getTail() != rhs.getTail();
}
/* --------------------------------------------------------------------------------------------- */

// <
/* --------------------------------------------------------------------------------------------- */
bool operator<(Tuple<> const&, Tuple<> const&)
{
    // empty tuples are always equivalent
    return false;
}

// recursive case
template<typename Head1, typename... Tail1,
         typename Head2, typename... Tail2,
         typename = enable_if_t<sizeof...(Tail1)==sizeof...(Tail2)>>
bool operator<(Tuple<Head1, Tail1...> const& lhs,
                Tuple<Head2, Tail2...> const& rhs)
{
    if (lhs.getHead() == rhs.getHead()) {
        return lhs.getTail() < lhs.getTail();
    }
    return lhs.getHead() < rhs.getHead();
}
/* --------------------------------------------------------------------------------------------- */
