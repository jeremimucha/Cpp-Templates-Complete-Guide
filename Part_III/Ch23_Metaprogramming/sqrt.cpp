#include <iostream>
#include <type_traits>
#include <utility>


// We could define `sqrt` template using partial specialization like so:
template<int N, int LO=1, int HI=N>
struct Sqrt_bad {
    // compute the mid point, rounded up
    static constexpr auto mid = (LO+HI+1)/2;

    // search a not too large value in a halved interval
    // Using the ternary operator here causes all branches to be evaluated (and instantiated),
    // which leads to template instantiation explosion.
    static constexpr auto value = (N<mid*mid) ? Sqrt_bad<N,LO,mid-1>::value
                                              : Sqrt_bad<N,mid,HI>::value;
};

// partial specialization for the case hen LO equals HI
template<int N, int M>
struct Sqrt_bad<N,M,M> { static constexpr auto value = M; };


// Recursive meta-template algorithms/implementations can lead to extensive template instantiations
// which can drastically increase compile times and compiler resource allocation.
// It can be avoided by ensuring lazy template instantiations - making sure that only the
// templates that we need instantiated are instantiated. We can utilize the fact that defining
// a type alias does not cause the obdy of that template to be instantiated.
template<int N, int LO=1, int HI=N>
struct Sqrt {
    // compute the midpoint, rounded up
    static constexpr auto mid = (LO+HI+1)/2;

    // search a not too large value in a halved interval
    using SubT = std::conditional_t<(N<mid*mid),        // Note that we don't immediately request
                                  Sqrt<N,LO,mid-1>,     // the resulting ::type, rather only
                                  Sqrt<N,mid,HI>>;      // select the template that meets the cond.

    static constexpr auto value = SubT::value;
};

// partial specialization to end the recursion
template<int N, int S>
struct Sqrt<N,S,S> {
    static constexpr auto value = S;
};


int main()
{

}
