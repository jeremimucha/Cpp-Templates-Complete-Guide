#include <iostream>
#include <type_traits>
#include <utility>


template<bool B, typename T = void>
struct enable_if { };

template<typename T>
struct enable_if<true,T> { using type = T; };

template<bool B, typename T = void>
using enable_if_t = typename enable_if<B,T>::type;


// std::enable_if can be used to enable/disable template implementations
// All the specializations need to have mutually exclusive enable_if conditions
// to ensure that no ambeguity errors occur during compilation.

template<typename T>
constexpr bool is_random_access_iterator =
    std::is_convertible_v<typename std::iterator_traits<T>::iterator_category,
                          std::random_access_iterator_tag>;

template<typename T>
constexpr bool is_bidirectional_iterator =
    std::is_convertible_v<typename std::iterator_traits<T>::iterator_category,
                          std::bidirectional_iterator_tag>;

// implementation for random access iterators
template<typename Iter, typename Distance>
enable_if_t<is_random_access_iterator<Iter>>
advance_iter(Iter& x, Distance n)
{
    x += n;
}

// implementation for bidirectional iterators
template<typename Iter, typename Distance>
enable_if_t<is_bidirectional_iterator<Iter> &&
                 !is_random_access_iterator<Iter>>
advance_iter(Iter& x, Distance n)
{
    if (n > 0) {
        for ( ; n > 0; ++x, --n) { }
    }
    else {
        for ( ; n < 0; --x, ++n) { }
    }
}

// implementation for all other iterators
template<typename Iter, typename Distance>
enable_if_t<!is_bidirectional_iterator<Iter>>
advance_iter(Iter& x, Distance n)
{
    if (n < 0) {
        throw "advance_iter(): invalid iterator category for negative n";
    }
    while (n > 0) {
        ++x;
        --n;
    }
}

int main()
{
    

}
