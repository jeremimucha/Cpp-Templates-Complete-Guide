#pragma once

#include <utility>
#include <type_traits>

// The `basic_tuple` implementation could be optimized for storage, runtime execution speed
// and compilation time.

// Optimizing for storage is achieved by utilizing the EBCO (empty base class optimization).
// Instead of having member objects we could inherit from the `tail` part.
namespace tuple_EBCO_first
{
    template<typename...> class Tuple;
    template<typename Head, typename... Tail>
    class Tuple<Head, Tail...> : private Tuple<Tail...>
    {
    private:
        Head head_;
        // ...
    };
} // tuple_EBCO_first
// This works but introduces a problem - Tail is initialized before head. This issue can be solved
// by introducing a `TupleElement` class that wraps each element type, so that Tuple can inherit
// from it. This lets us inherit from both Head and Tail.

namespace tuple_EBCO_second
{
    template<typename...> class Tuple;

    template<typename T>
    class TupleElement
    {
    private:
        T value_;
    public:
        TupleElement() = default;

        template<typename U>
        TupleElement(U&& other) : value_{std::forward<U>(other)} { }

        T& get() { return value_; }
        T const& get() const { return value_; }
    };

    template<typename Head, typename... Tail>
    class Tuple<Head, Tail...> : private TupleElement<Head>, private Tuple<Tail...>
    {
    public:
        Head& getHead() {
            // potentially ambiguous if two elements have the same type -> Tuple<int, double, int>
            return static_cast<TupleElement<Head>*>(this)->get();
        }
    // ...
    };
} // tuple_EBCO_second
// This naive implementation introduces a problem - if the Tuple holds more than one element of
// the same type we can no longer extract elements from the tuple.

// This problem is solved by ensuring that each TupleElement base class is unique within a given
// Tuple. One approach is to encode the index (or "height") of a value within the tuple.
// The last element will be stored at height 0, second to last at height 1, and so on.
namespace tuple_EBCO_third
{
    template<unsigned Height, typename T>
    class TupleElement {
    private:
        T value_;
    public:
        TupleElement() = default;

        template<typename U>
        TupleElement(U&& other) : value_{std::forward<U>(other)} { }

        T& get() { return value_; }
        T const& get() const { return value_; }
    };
// This lets us implement a Tuple that applies EBCO while maintaining intialization order and
// supports multiple elements of the same type:
    template<typename...> class Tuple;

    template<typename Head, typename... Tail>
    class Tuple<Head,Tail...>
        : private TupleElement<sizeof...(Tail), Head>, private Tuple<Tail...>
    {
        using HeadElement = TupleElement<sizeof...(Tail), Head>;
    public:
        Head& getHead() {
            return static_cast<HeadElement*>(this)->get();
        }
        Head const& getHead() const {
            return static_cast<HeadElement const*>(this)->get();
        }
        Tuple<Tail...>& getTail() { return *this; }
        Tuple<Tail...> const& getTail() const { return *this; }
    };

    template<>
    class Tuple<> { };
} // tuple_EBCO_third
// The TupleElement could be optimized further by implementing it using inheritance aswell.
// This can be applied only for cases where the wrapped type is a non-final class;

namespace tuple_EBCO_fourth
{
    template<unsigned Height, typename T,
             bool = std::is_class_v<T> && !std::is_final_v<T>>
    class TupleElement;

    template<unsigned Height, typename T>
    class TupleElement<Height,T,false>
    {
        // Same implementation as before
    };

    template<unsigned Height, typename T>
    class TupleElement<Height,T,true> : private T
    {
    public:
        TupleElement() = default;
        template<typename U>
        TupleElement(U&& other) : T{std::forward<U>(other)} { }
    
        T& get() { return *this; }
        T const& get() const { return *this; }
    };
} // tuple_EBCO_fourth
// This implementation also optimizes oway (space-wise) any elements which are empty classes.

// Introduction of the TupleElement also allows for a optimized implementation of get<>().
// Calculating the elements "height" will let us rely on conversion from Tuple to the given
// base class TupleElement<H,T>, where T is deduced.

namespace tuple_get_optimization
{
    template<unsigned H, typename T>
    T& getHeight(TupleElement<H,T>& te)
    {
        return te.get();
    }

    template<typename...> class Tuple;

    template<unsigned I, typename... Elements>
    auto get(Tuple<Elements...>& t) -> decltype(getHeight<sizeof...(Elements)-I-1>(t))
    {
        return getHeight<sizeof...(Elements)-I-1>(t);
    }
} // tuple_get_optimization
