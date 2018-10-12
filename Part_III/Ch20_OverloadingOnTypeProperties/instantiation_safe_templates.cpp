#include <iostream>
#include <type_traits>
#include <utility>


// The idea behind instantiation-safe templates is to ensure that every operation used by the
// template and every other requirement is encoded/checked as part of the enable_if condition.
// This would ensure that if a template is instantiated an error can not occur deep in it's
// body,thus avoiding the "error novel" that compilers produce in those cases.

// Given:
template<typename T>
T const& min_(T const& x, T const& y)
{
    if (y < x) { return y; }
    return x;
}
// The template requires T to have a `<` operator with the result implicitly convertible to bool.

template<typename...> using void_t = void;

template<typename T1, typename T2, typename = void>
struct has_less : std::false_type { };

template<typename T1, typename T2>
using less_expression = decltype(std::declval<T1>() < std::declval<T2>());

template<typename T1, typename T2>
struct has_less<T1, T2, void_t<less_expression<T1,T2>>> : std::true_type { };

template<typename T1, typename T2>
constexpr bool has_less_v = has_less<T1,T2>::value;

template<typename T1, typename T2, bool = has_less_v<T1,T2>>
struct less_result_impl { using type = less_expression<T1,T2>; };

template<typename T1, typename T2>
struct less_result_impl<T1,T2,false> { };

template<typename T1, typename T2>
struct less_result : less_result_impl<T1,T2> { };

template<typename T1, typename T2>
using less_result_t = typename less_result<T1,T2>::type;

// Nowe we can use has_less and less_result_t to check if the template arguments meet requirements
template<typename T>
std::enable_if_t<std::is_convertible_v<less_result_t<T const&, T const&>, bool>,
                 T const&>
min(T const& x, T const& y)
{
    if (y < x) { return y; }
    return x;
}


int main()
{

}
