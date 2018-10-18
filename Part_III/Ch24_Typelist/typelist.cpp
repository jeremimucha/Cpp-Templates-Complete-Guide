#include <iostream>
#include <utility>
#include "typelist.hpp"

// unit tests
namespace ut_back
{
    using stuff = typelist<int, char, double>;
    using two_elem = typelist<int, float>;
    using one_elem = typelist<double>;

    static_assert(std::is_same_v<back_t<stuff>,double>,
        "back_t: three elem list case failed");
    static_assert(std::is_same_v<back_t<one_elem>, double>,
        "back_t: single element list case failed");
    static_assert(std::is_same_v<back_t<two_elem>, float>,
        "back_t: two elem list case failed");

    static_assert(std::is_same_v<front_t<stuff>, int>,
        "front_t: three elem list case failed");
    static_assert(std::is_same_v<front_t<one_elem>, double>,
        "front_t: single elem list case failed");
    static_assert(std::is_same_v<front_t<two_elem>, int>, "front_t: two elem list case failed");
} // namespace ut_back

namespace ut_push_front_push_back
{
    using lst1 = typelist<int, char>;
    using lst1_prim = push_front_t<lst1, double>;
    using lst1_prim_lazy = push_front<lst1, double>;
    using lst1_expected = typelist<double, int, char>;
    static_assert(std::is_same_v<lst1_expected, lst1_prim>, "push_front test failed");
    static_assert(std::is_same_v<lst1_expected, typename lst1_prim_lazy::type>,
        "lazy push_front test failed");

    using lst_empty = typelist<>;
    using lst_empty_prim = push_front_t<lst_empty, double>;
    using lst_empty_prim_lazy = push_front<lst_empty, double>;
    using lst_empty_expected = typelist<double>;
    static_assert(std::is_same_v<lst_empty_expected, lst_empty_prim>,
        "push_front empty list test failed");
    static_assert(std::is_same_v<lst_empty_expected, typename lst_empty_prim_lazy::type>,
        "lazy push_front empty list test failed");

    using lst2 = typelist<int>;
    using lst2_prim = push_front_t<lst2, char, double>;
    using lst2_prim_lazy = push_front<lst2, char, double>;
    using lst2_expected = typelist<char, double, int>;
    static_assert(std::is_same_v<lst2_expected, lst2_prim>,
        "push_front empty list test failed");
    static_assert(std::is_same_v<lst2_expected, typename lst2_prim_lazy::type>,
        "lazy push_front empty list test failed");

    using lst2_pback = push_back_t<lst2, char, double>;
    using lst2_pback_lazy = push_back<lst2, char, double>;
    using lst2_pback_expected = typelist<int, char, double>;
    static_assert(std::is_same_v<lst2_pback_expected, lst2_pback>, "push_back failed");
    static_assert(std::is_same_v<lst2_pback_expected, typename lst2_pback_lazy::type>,
        "lazy push_back failed");
    using lst_empty_pback = push_back_t<lst_empty, double, int>;
    using lst_empty_pback_lazy = push_back<lst_empty, double, int>;
    using lst_empty_pback_expected = typelist<double, int>;
    static_assert(std::is_same_v<lst_empty_pback_expected, lst_empty_pback>, "push_back failed");
    static_assert(std::is_same_v<lst_empty_pback_expected, typename lst_empty_pback_lazy::type>,
        "lazy push_back failed");
} // namespace ut_push_back


namespace ut_pop_front
{
    using lstpop1 = typelist<int, char>;
    using lstpop1_prim = pop_front_t<lstpop1>;
    using lstpop1_prim_lazy = pop_front<lstpop1>;
    using lstpop1_expected = typelist<char>;
    static_assert(std::is_same_v<lstpop1_prim, lstpop1_expected>, "pop_front test 1 failed");
    static_assert(std::is_same_v<typename lstpop1_prim_lazy::type, lstpop1_expected>,
        "lazy pop_front test 1 failed");

    using lstpop2 = typelist<int>;
    using lstpop2_prim = pop_front_t<lstpop2>;
    using lstpop2_prim_lazy = pop_front<lstpop2>;
    using lstpop2_expected = typelist<>;
    static_assert(std::is_same_v<lstpop2_prim, lstpop2_expected>, "pop_front test 2 failed");
    static_assert(std::is_same_v<typename lstpop2_prim_lazy::type, lstpop2_expected>,
        "lazy pop_front test 2 failed");
} // namespace ut_pop_front


namespace ut_count
{
    using lstcnt0 = typelist<>;
    using lstcnt3 = typelist<int, char, double>;
    static_assert(count_v<lstcnt0> == 0ull, "count failed for empty list");
    static_assert(count_v<lstcnt3> == 3ull, "count failed for list of size 3");

    using lst_cns_0 = typelist<>;
    using lst_cns_3 = typelist<int, double, char>;
    static_assert(contains_v<lst_cns_0, double> == false, "contains failed for empty list");
    static_assert(contains_v<lst_cns_3, double> == true, "contains failed for list of size 3");
    static_assert(contains_v<lst_cns_3, float> == false,
        "contains failed negative test for a list of size 3");
} // namespace ut_count


namespace ut_contains
{
    using test_variant = typelist<int, double, const char*>;
    static_assert(contains_v<test_variant, int>, "contains failed for variant");
    static_assert(contains_v<test_variant, const char*>,
        "contains failed for variant const char*");
    static_assert(contains_v<test_variant, char> == false,
        "contains failed negative test for variant");
} // namespace ut_contains

int main()
{

}
