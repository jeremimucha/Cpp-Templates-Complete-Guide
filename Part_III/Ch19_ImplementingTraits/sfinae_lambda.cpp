#include <iostream>
#include <utility>


// -------------------------------- SFINAE function overloading -----------------------------------
// helper: checking validity of f(args...) for F f and Args... args:
template<typename F, typename... Args,
         typename = decltype(std::declval<F>()(std::declval<Args&&>()...))>
constexpr std::true_type is_valid_impl(void*);

// fallback if heler SFINAE'd out:
template<typename F, typename... Args>
constexpr std::false_type is_valid_impl(...);

// define a lambda that takes a lambda f and returns whether calling f with args is valid
inline constexpr auto is_valid =
    [](auto f){
        return [](auto&&... args){
            return decltype(is_valid_impl<decltype(f), decltype(args)&&...>(nullptr)){};
        };
    };

// helper template to represent a type as a value
template<typename T>
struct type_t { using type = T; };

// helper to wrap a type as a value
template<typename T>
constexpr auto type = type_t<T>{};

// helper to unwrap a wrapped type in unevaluated contexts
template<typename T>
constexpr T value_t(type_t<T>);   // no definition needed

constexpr auto is_default_constructible =
    is_valid( [](auto x) -> decltype( (void)decltype(value_t(x))() ) { } );


int main()
{
    std::cout << std::boolalpha;
    std::cout << "is_default_constructible(type<int>) = "
        << is_default_constructible(type<int>) << "\n";
    std::cout << "is_default_constructible(type<int&>) = "
        << is_default_constructible(type<int&>) << "\n";
}
