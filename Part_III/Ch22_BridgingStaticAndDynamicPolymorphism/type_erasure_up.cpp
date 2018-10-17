#include <iostream>
#include "functionptr_uniqueptr.hpp"


void foo(int i) { std::cout << __PRETTY_FUNCTION__ << ", " << i << '\n'; }

struct bar
{
    void operator()(int i) const noexcept
        { std::cout << __PRETTY_FUNCTION__ << ", " << i << '\n'; }
};


int main()
{
    constexpr auto lam = [](int i){ std::cout << __PRETTY_FUNCTION__ << ", " << i << '\n'; };
    FunctionPtr<void(int)> fp1;    // nullptr
    FunctionPtr<void(int)> fp2{lam};
    std::cout << std::boolalpha;
    std::cout << "fp1 == fp2: " << (fp1 == fp2) << '\n';
    fp2(42);
    fp1 = lam;
    std::cout << "after fp1 = lam; fp1 == fp2: " << (fp1 == fp2) << '\n';
    fp2 = foo;
    fp1 = &foo;
    std::cout << "after fp1 = foo; fp2 = &foo; fp1 == fp2: " << (fp1 == fp2) << '\n';
    fp1 = bar{};
    fp2 = bar{};
    try {
        fp1 == fp2;
    }
    catch(not_equality_comparable const& e) {
        std::cout << "caught exception: struct bar doesn't define operator==" << '\n';
    }
}
