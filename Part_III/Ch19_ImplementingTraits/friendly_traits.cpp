#include <iostream>
#include <string>
#include <type_traits>
#include <utility>


/**
 * We should try to make all type traits (SFINAE) friendly. Meaning that if a type doesn't meet
 * the requirements of a type trait it shouldn't immediately cause the entire program to become
 * ill-formed during template instantiation / parameter substitution.
 * We can achieve that by testing for the condition / expression we're using in a SFINAE context.
 */

// For example, given:
template<typename T1, typename T2>
struct plus_result_bad {
    using type = decltype(std::declval<T1>() + std::declval<T2>());
};
// The `+` is used in a context that is not protected by SFINAE which can cause a program
// attempting to evaluate plus_result to become ill-formed if operator+ isn't defined for T1 and T2

// The solution is to detect first if there is a suitable operator+
using std::void_t;

template<typename T1, typename T2>
using plus_expression = decltype(std::declval<T1>() + std::declval<T2>());

template<typename, typename, typename = void_t<>>
struct has_plus : std::false_type { };

template<typename T1, typename T2>
struct has_plus<T1, T2, void_t<plus_expression<T1,T2>>>
        : std::true_type { };

// plus_result then becomes:
// primary template, used when has_plus yields true
template<typename T1, typename T2, bool = has_plus<T1,T2>::value>
struct plus_result { using type = plus_expression<T1,T2>; };

// partial specialization used otherwise
template<typename T1, typename T2>
struct plus_result<T1, T2, false> { };  // no `type` defined

static_assert(has_plus<double, int>::value, "has_plus test failed");
static_assert(has_plus<int,std::string>::value == false, "has_plus test failed");


int main()
{

}
