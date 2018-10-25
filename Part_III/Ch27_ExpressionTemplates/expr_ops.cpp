#include <iostream>
#include "expr_array.hpp"
#include "expr_ops.hpp"


int main()
{
    Array<double> x{1000}, y{1000};
    x = 1.2*x + x*y;

    Array<int> a{100}, b{100};
    a[b] = 2*a[b];
}
