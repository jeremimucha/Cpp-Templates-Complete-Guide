#include <iostream>
#include <type_traits>
#include <utility>


// if_then_else - implementation of std::conditional
template<bool Cond, typename TrueType, typename FalseType>
struct if_then_else { using type = TrueType; };

template<typename TrueType, typename FalseType>
struct if_then_else { using type = FalseType; };

template<bool C, typename T, typename F>
using if_then_else_t = typename if_then_else<C,T,F>::type;


// We need to be careful when in some applications of type traits
// as some may result in undefined behavior.
// For example std::make_unsigned requires that the passed type is a signed integral type
// and not bool; otherwise it results in undefined behavior.
// In such cases we should implement a trait that yields the corresponsing unsigned type
// if possible and the passed type otherwise.

// The naive implementation doesn't work:
template<typename T>
struct unsigned_t_bad {
    using type = if_then_else_t<std::is_integral_v<T> && !std::is_same_v<T,bool>,
                                std::make_unsigned_t<T>,
                                T>;
};
// The above still results in undefined behavior because ALL if_then_else_t arguments are
// fully instantiated and evaluated.

// To avoid this issue we need an additional level of indirection. We can achieve that by wrapping
// the result types in templates
template<typename T>
struct identity { using type = T; };

template<typename T>
struct make_unsigned_indentity { using type = typename std::make_unsigned<T>::type; };

template<typename T>
struct unsigned_t {
    using type = typename if_then_else<std::is_integral_v<T> && !std::is_same_v<T,bool>,
                                       make_unsigned_indentity<T>,
                                       identity<T>,
                                       >::type;
};
// In the above case the type functions are not actually fully evaluated untill if_then_else
// selects one. Only after one of the results is selected is the ::type applied to it, thus
// avoiding undefined behavior.

int main()
{

}
