#include <iostream>
#include <array>
#include <type_traits>
#include <utility>

using std::size_t;

// using constexpr
template<typename T, size_t N>
constexpr auto dot_product(std::array<T,N> const& x, std::array<T,N> const& y) noexcept
{
    auto res = x.front() * y.front();
    for ( size_t i = 1; i < N; ++i) {
        res += x[i] * y[i];
    }
    return res;
}

// Using partial specialization, avoids the loop:
template<typename T, size_t N>
struct DotProduct {
    static constexpr T result(const T* a, const T* b) {
        return *a * *b + DotProduct<T, N-1>::result(a+1, b+1);
    }
};

template<typename T>
struct DotProduct<T, 0> {
    static constexpr T result(const T*, const T*) {
        return T{};
    }
};

template<typename T, std::size_t N>
constexpr auto dotProduct(std::array<T,N> const& x, std::array<T,N> const& y) noexcept
{
    return DotProduct<T,N>::result(x.begin(), y.begin());
}

constexpr std::array<int, 3> coll1 {1,2,3};
constexpr std::array<int, 3> coll2 {9,8,7};

constexpr auto prod1 = dot_product(coll1, coll2);
constexpr auto prod2 = dotProduct(coll1, coll2);

int main()
{
    std::cout << "coll1: ";
    for (auto e : coll1) { std::cout << e << ", "; }
    std::cout << "\ncoll2: ";
    for (auto e : coll2) { std::cout << e << ", "; }
    std::cout << '\n';
    std::cout << "prod1 = " << prod1 << '\n';
    std::cout << "prod2 = " << prod2 << '\n';

}
