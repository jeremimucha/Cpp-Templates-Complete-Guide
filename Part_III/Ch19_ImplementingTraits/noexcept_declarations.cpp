#include <iostream>
#include <vector>
#include <type_traits>
#include <utility>

using std::void_t;


// When making conditional noexcept declarations we should also make sure that we're using
// `friendly traits` that don't result in the program becoming ill-formed

// Define is_nothrow_move_constructible that handles types without move constructors
// primary template
template<typename T, typename = void_t<>>
struct is_nothrow_move_constructible : std::false_type { };

// partial specialization (may be SFINAE'd away)
template<typename T>
struct is_nothrow_move_constructible<T, void_t<decltype(T(std::declval<T&&>()))>>
    : std::bool_constant<noexcept(T(std::declval<T&&>()))> { };

static_assert(is_nothrow_move_constructible<std::vector<int>>::value);


int main()
{

}
