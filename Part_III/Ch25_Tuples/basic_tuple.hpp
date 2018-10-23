#pragma once

#include <utility>
#include <type_traits>


template<bool B, typename T = void>
struct enable_if { };

template<typename T>
struct enable_if<true,T> { using type = T; };

template<bool B, typename T = void>
using enable_if_t = typename enable_if<B,T>::type;


// The idea is to define a type recursively, much like a type list.
// A Tuple of N (> 0) elements can be defined as the Nth element and a tuple of N-1 elements,
// with a special case for a tuple of 0 elements.
template<typename... Types>
class Tuple;

// recursive case
template<typename Head, typename... Tail>
class Tuple<Head, Tail...>
{
private:
    Head head_;
    Tuple<Tail...> tail_;
public:
    // constructors:
    Tuple() = default;

    template<typename VHead, typename... VTail,
             typename = enable_if_t<sizeof...(VTail)==sizeof...(Tail)>>
    Tuple(VHead&& head, VTail&&... tail)
        : head_{std::forward<VHead>(head)}, tail_{std::forward<VTail>(tail)...} { }

    template<typename VHead, typename... VTail,
             typename = enable_if_t<sizeof...(VTail)==sizeof...(Tail)>>
    Tuple(Tuple<VHead,VTail...> const& other)
        : head_{other.getHead()}, tail_{other.getTail()} { }

    template<typename T, typename... VTuple>
    Tuple(T&& t, Tuple<VTuple...> const& tup)
        : head_{std::forward<T>(t)}, tail_{tup} { }

    Head& getHead() { return head_; }
    Head const& getHead() const { return head_; }
    Tuple<Tail...>& getTail() { return tail_; }
    Tuple<Tail...> const& getTail() const { return tail_; }
};

// basis case:
template<>
class Tuple<> {
    // no storage required
};


// Getting elements by index
template<unsigned N>
struct TupleGet {
    template<typename Head, typename... Tail>
    static auto apply(Tuple<Head, Tail...> const& t) {
        return TupleGet<N-1>::apply(t.getTail());
    }
};

template<>
struct TupleGet<0> {
    template<typename Head, typename... Tail>
    static Head const& apply(Tuple<Head,Tail...> const& t) {
        return t.getHead();
    }
};


// element type deduction
template<typename... Types>
auto makeTuple(Types&&... elems)
{
    return Tuple<std::decay_t<Types>...>(std::forward<Types>(elems)...);
}
