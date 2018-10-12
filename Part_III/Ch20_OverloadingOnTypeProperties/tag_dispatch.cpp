#include <iostream>
#include <type_traits>
#include <iterator>
#include <vector>
#include <list>
#include <utility>


// one of the approaches to algorithm specialization is to "tag" different implementations
// with a unique type that identifies the variant. It's done by introducing an extra parameter
// of the tag type
template<typename Iterator, typename Distance>
void advance_iter_impl(Iterator& x, Distance n, std::input_iterator_tag)
{
    std::cout << "advance_iter - input_iterator_tag variant\n";
    while (n > 0) {
        ++x;
        --n;
    }
}

template<typename Iterator, typename Distance>
void advance_iter_impl(Iterator& x, Distance n, std::random_access_iterator_tag)
{
    std::cout << "advance_iter - random_access_iterator_tag variant\n";
    x += n;
}

template<typename Iterator, typename Distance>
void advance_iter(Iterator& x, Distance n)
{
    advance_iter_impl(x, n, typename std::iterator_traits<Iterator>::iterator_category{});
}


int main()
{
    std::vector<int> vec{1,2,3,4};
    std::list<int> lst{1,2,3,4};
    auto vec_it = vec.begin();
    auto lst_it = lst.begin();
    std::cout << "calling advance_iter on vec_it:\n";
    advance_iter(vec_it, 2);
    std::cout << "calling advance_iter on lst_it:\n";
    advance_iter(lst_it, 3);
}
