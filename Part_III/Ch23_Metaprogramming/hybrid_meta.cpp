#include <iostream>
#include <type_traits>
#include <utility>


// ratio
template<unsigned N, unsigned D = 1>
struct Ratio {
    static constexpr unsigned num = N;      // numerator
    static constexpr unsigned den = D;      // denominator
    using type = Ratio;
};

// Compile time add two units
template<typename R1, typename R2>
struct RatioAddImpl
{
private:
    static constexpr unsigned den = R1::den * R2::den;
    static constexpr unsigned num = R1::num * R2::den + R2::num * R1::den;
public:
    using type = Ratio<num, den>;
};

template<typename R1, typename R2>
using RatioAdd = typename RatioAddImpl<R1, R2>::type;


// Duration type for values of type T iwth unit type U:
template<typename T, typename U = Ratio<1>>
class Duration
{
public:
    using value_type = T;
    using unit_type = typename U::type;

    constexpr Duration() noexcept = default;
    constexpr Duration(value_type v) noexcept
        : val_{v} { }

    constexpr value_type value() const noexcept { return val_; }
private:
    value_type val_{0};
};

// adding two durations where unit type might differ
template<typename T1, typename U1, typename T2, typename U2>
constexpr auto operator+(Duration<T1,U1> const& lhs, Duration<T2,U2> const& rhs) noexcept
{
    // resulting type is a unit with 1 a nominator and the resulting denominator
    // of adding both unit type fractions
    using VT = Ratio<1, RatioAdd<U1,U2>::den>;

    // resulting value is the sum of both values
    // converted to the resulting unit type
    auto val = lhs.value() * VT::den / U1::den * U1::num +
               rhs.value() * VT::den / U2::den * U2::num;
    return Duration<decltype(val), VT>{val};
}

int main()
{
    // Adding ratios
    using R1 = Ratio<1,1000>;
    using R2 = Ratio<2,3>;
    using RS = RatioAdd<R1,R2>;
    std::cout << "RS = " << RS::num << '/' << RS::den << '\n';      // prints 2003/3000


    int x = 42;
    int y = 77;
    auto a = Duration<int, Ratio<1,1000>>{x};   // x milliseconds
    auto b = Duration<int, Ratio<2,3>>{y};      // y 2/3 seconds
    auto c = a + b;     // computes resulting unit type 1/3000 seconds
    std::cout << "result = " << c.value() << " * "
        << decltype(c)::unit_type::num << '/' << decltype(c)::unit_type::den << '\n';
}
