#include <iostream>
#include "simple_array.hpp"
#include "simple_ops.hpp"


int main()
{
    SArray<double> x{1000}, y{1000};
    x = 1.2*x + x*y;
    // The above is very inefficient for SArray
    // At least 3 temporary arrays are created
    // tmp1 = 1.2*x     -- loop of 1000 operations, plus creation and destruction of tmp1
    // tmp2 = x*y       -- loop of 1000 operations, plus ctor/dtor of tmp2
    // tmp3 = tmp1+tmp2 -- loop of 1000 operations, plux ctor/dtor of tmp3
    // x = tmp3;        -- 1000 read operations and 1000 write operations
}
