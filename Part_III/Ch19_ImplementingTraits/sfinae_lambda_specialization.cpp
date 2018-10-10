#include <iostream>
#include <string>
#include <utility>


// ------------------------------- SFINAE partial specialization ----------------------------------
template<typename...> using void_t = void;

// primary template if specialization SFINAE'd out:
template<typename F, typename Void, typename... Args>
struct is_valid_impl : std::false_type { };

// specialization checking validity of f(args...) for F f and Args... args:
template<typename F, typename... Args>
struct is_valid_impl<F,
                     void_t<decltype(std::declval<F>()(std::declval<Args&&>()...))>,
                     Args...
                     > : std::true_type { };

inline constexpr auto is_valid =
    [](auto f){
        return [](auto&&... args){
            return is_valid_impl<decltype(f), void, decltype(args)&&...>::value;
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
constexpr T value_t(type_t<T>);   // no definition neede

constexpr auto is_default_constructible =
    is_valid( [](auto x) -> decltype( (void)decltype(value_t(x)){} ) { } );
                                        // The (void) cast is on the default constructed value of
                                        // type extracted by decltype(value_t(x)) to idicate
                                        // that it is unused

int main()
{
    std::cout << std::boolalpha;
    std::cout << "is_default_constructible(type<int>) = "
        << is_default_constructible(type<int>) << "\n";
    std::cout << "is_default_constructible(type<int&>) = "
        << is_default_constructible(type<int&>) << "\n";
    std::cout << "is_default_constructible(type<std::string>) = "
        << is_default_constructible(type<std::string>) << "\n";
}
