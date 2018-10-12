#include <iostream>
#include <type_traits>
#include <utility>


template<typename T>
constexpr bool is_random_access_iterator =
    std::is_convertible_v<typename std::iterator_traits<T>::iterator_category,
                          std::random_access_iterator_tag>;

template<typename T>
constexpr bool is_bidirectional_iterator =
    std::is_convertible_v<typename std::iterator_traits<T>::iterator_category,
                          std::bidirectional_iterator_tag>;

template<typename T>
constexpr bool is_input_iterator =
    std::is_convertible_v<typename std::iterator_traits<T>::iterator_category,
                          std::input_iterator_tag>;

// When defining constructors it is impossible to place enable_if as the return type.
// In other cases it also might be undesireable for readability reasons.
// Another common case is to place the enable_if expression as the last defaulted template argument

template<typename T>
class Container
{
public:
    // construct from an input iterator sequence:
    template<typename Iter,
             typename std::enable_if_t<is_input_iterator<Iter> &&
                                       !is_random_access_iterator<Iter>>>
    Container(Iter first, Iter last);

    // To avoid a redeclaration error we introduce an extra "dummy" template parameter here,
    // otherwise both constructor would have had the same signature
    template<typename Iter,
             typename std::enable_if_t<is_random_access_iterator<Iter>>,
             typename = int>    // extra dummy parameter to enable both constructors
    Container(Iter first, Iter last);

    // convert to another container as long as the value types are convertible
    template<typename U, typename = std::enable_if_t<std::is_convertible_v<T,U>>>
    operator Container<U>() const;
};


int main()
{

}
