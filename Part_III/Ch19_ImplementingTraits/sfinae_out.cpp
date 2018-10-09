#include <iostream>
#include <string>
#include <type_traits>
#include "issame.hpp"


// -------------------------------- SFINAE function overloading -----------------------------------
// The basic idea is to use SFINAE to detect if an expression is valid
// This can be done using overload resolution:
// if an expression is valid - the overload returning some type (here char)
// is selected; if it isn't - a fallback overload (taking c-style varargs - ... elipsis, the least
// preffered match) is selected, by checking which result type we got we can determine if
// the expression is valid (here default construction).
template<typename T>
struct IsDefaultConstructibleT_first {
private:
    // test() trying substitute call of a default constructo for T passed as U
    template<typename U, typename = decltype(U())>
    static constexpr char test(void*);

    // test() fallback:
    template<typename>
    static constexpr long test(...);
public:
    static constexpr bool value = issame<decltype(test<T>(nullptr)), char>::value;
};

// An improvement would be to change the return types to std::true_type and std::false_type
template<typename T>
struct IsDefaultConstructibleT_impl {
private:
    template<typename U, typename = decltype(U())>
    static constexpr std::true_type test(void*);
    // fallback:
    template<typename>
    static std::false_type test(...);
public:
    using type = decltype(test<T>(nullptr));
};

template<typename T>
struct IsDefaultConstructibleT : IsDefaultConstructibleT_impl<T>::type { };
// ------------------------------------------------------------------------------------------------


// ------------------------------- SFINAE partial specialization ----------------------------------
// use the glorious void_t...
template<typename...>
using void_t = void;

template<typename T, typename = void>
struct is_default_constructible_impl : std::false_type { };

template<typename T>
struct is_default_constructible_impl<T, void_t<decltype(T())>> : std::true_type { };

template<typename T>
struct is_default_constructible : is_default_constructible_impl<T> { };

template<typename T>
using is_default_constructible_t = typename is_default_constructible<T>::type;

template<typename T>
static constexpr auto is_default_constructible_v = is_default_constructible<T>::value;
// ------------------------------------------------------------------------------------------------



int main()
{
    std::cout << "std::string is default constructible = " << std::boolalpha
        << is_default_constructible_v<std::string> << "\n";
}
