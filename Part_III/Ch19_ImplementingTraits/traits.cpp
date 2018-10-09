#include <iostream>


// simple implementation of accumulate, not yet considering customization through traits
// This has some issues - for e.g. T might be too small to accumulate a range of T values,
// or it might not have the desired default value...
template<typename T>
T accum_simple(T const* beg, T const* end)
{
    T total{};  // assume this actually creates a zero value
    while (beg != end) {
        total += *beg;
        ++beg;
    }
    return total;
}


// Which is where traits come in
// We can write a template which via full or partial specialization maps appropriate
// types and default values for the `total` variable.
template<typename T>
struct AccumulateTraits {   // we could choose not to provide a default implementation
    using AccT = T;
    // C++17, older code might need to define this in a .cpp source file
    inline static constexpr AccT zero{};
    // pre C++17 alternative is to use static inline functions that return the default value
    // static constexpr AccT zero() { return 0; }
};

template<>
struct AccumulateTraits<char> {
    using AccT = int;
    inline static constexpr AccT zero = 0;
};

template<>
struct AccumulateTraits<short> {
    using AccT = int;
    inline static constexpr AccT zero = 0;
};

template<>
struct AccumulateTraits<int> {
    using AccT = long;
    inline static constexpr AccT zero = 0;
};

template<>
struct AccumulateTraits<unsigned int> {
    using AccT = unsigned long;
    inline static constexpr AccT zero = 0;
};

template<>
struct AccumulateTraits<float> {
    using AccT = double;
    inline static constexpr AccT zero = 0.0;
};

// other specializations, also for class types etc...

// Given the above traits accum can more precicely control the result type and starting value
template<typename T>
auto accum_with_traits(T const* beg, T const* end)
{
    using AccT = typename AccumulateTraits<T>::AccT;
    AccT total = AccumulateTraits<T>::zero; // init total by trait value
    while (beg != end) {
        total += *beg;
        ++beg;
    }
    return total;
}

// To allow for further customization through traits we can parameterize the trait
// by making it a default template parameter. A user could in this case define a different set
// of traits if desired
template<typename T, typename AT = AccumulateTraits<T>>
auto accum_param(T const* beg, T const* end)
{
    typename AT::AccT total = AT::zero;
    while (beg != end) {
        total += *beg;
        ++beg;
    }
    return total;
}

// We could observe that the algorithm could be customized further by providing a way to use
// a different operation in place of `total += *beg`.
// This operation is called a `policy`. We can provide different policies in much the same
// way we provide traits - via classes or class templates. Policy action can be a static function
// of a class or template...
struct SumPolicy {
    template<typename T1, typename T2>
    static constexpr void accumulate(T1& total, T2 const& value) {
        total += value;
    }
};

struct MultPolicy {
    template<typename T1, typename T2>
    static constexpr void accumulate(T1& total, T2 const& value) {
        total *= value;
    }
};

// We customize the algorithm by adding the ability to specify the policy
template<typename T,
         typename Policy = SumPolicy,
         typename Traits = AccumulateTraits<T>>
auto accum_policy(T const* beg, T const* end)
{
    using AccT = typename Traits::AccT;
    AccT total = Traits::zero;
    while (beg != end) {
        Policy::accumulate(total, *beg);
        ++beg;
    }
    return total;
}

// Above we chose to templetize the Policy::accumulate function, rather ten the policy class.
// We could define the policy to be a class template instead, this provides the opportinuty
// to, for example, store some state in the policy, in which case the type would most likely be
// important. This would require a different declaration of accum itself
template<typename T1, typename T2>
struct SumPolicyTmplt {
    static constexpr void accumulate(T1& total, T2 const& value) {
        total += value;
    }

    // some default (static?) members here...
};

// accum then becomes:
template<typename T,
         template<typename,typename> class Policy = SumPolicyTmplt,
         typename Traits = AccumulateTraits<T>>
constexpr auto accum_tmplt_policy(T const* beg, T const* end)
{
    using AccT = typename Traits::AccT;
    AccT total = Traits::zero;
    while (beg != end) {
        Policy<AccT,T>::accumulate(total, *beg);
        ++beg;
    }
    return total;
}

int main()
{
    int num[] = {1, 2, 3, 4, 5 };

    std::cout << "product of all values = "
        << accum_policy<int, MultPolicy>(num, num+5) << "\n";
    // The above always prints `0` because we use 0 for the initial total value.
    // This could be solved in various ways via Policies or Traits... but it doesn't have to be
    // the STL std::accumulate takes the initial value as a third (function call) argument.

    std::cout << "sum of all values = "
        << accum_tmplt_policy(num, num+5) << "\n";
}
