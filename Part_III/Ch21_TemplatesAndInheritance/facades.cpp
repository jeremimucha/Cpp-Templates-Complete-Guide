#include <iostream>
#include <type_traits>


// Crtp can be used to implement the `facade` pattern - implement most or all of the public
// interface of a class in terms of a much smaller (but easier to implement) interface exposed
// by the CRTP derived class.
// Illustraded below by an implementation of a facade for iterators
template<typename Derived, typename Value, typename Category,
         typename Reference = std::add_lvalue_reference_t<Value>,
         typename Distance = std::ptrdiff_t>
class IteratorFacade
{
public:
    using value_type = std::remove_const_t<Value>;
    using reference = Reference;
    using pointer = std::add_pointer_t<Value>;
    using difference_type = Distance;
    using iterator_category = Category;

// input iterator interface
    reference operator*() const { return derived().dereference(); }
    pointer operator->() const { return &(this->operator*()); }
    Derived& operator++()
    {
        derived().increment();
        return derived();
    }
    Derived operator++(int) const
    {
        const auto res{derived()};
        derived().increment();
        return res;
    }
    friend bool operator==(IteratorFacade const& lhs, IteratorFacade const& rhs) noexcept
    {
        return lhs.derived().equals(rhs.derived());
    }

// bidirectional iterator interface
    Derived& operator--()
    {
        derived().decrement();
        return derived();
    }
    Derived operator--(int) const
    {
        const auto res{derived()};
        derived().decrement();
        return res;
    }

// random access iterator interface
    reference operator[](difference_type n) const { return *(derived() + n); }
    Derived& operator+=(difference_type n) { derived().advance(n); return derived(); }
    Derived operator+(difference_type n) const { auto res{derived()}; return res += n; }
    Derived& operator-=(difference_type n) { derived().advance(-n); return derived(); }
    Derived operator-(difference_type n) const { auto res{derived()}; return res -= n; }
    friend difference_type operator-(IteratorFacade const& lhs, IteratorFacade const& rhs) noexcept
    {
        return lhs.derived().difference_from(rhs.derived());
    }

    friend bool operator<(IteratorFacade const& lhs, IteratorFacade const& rhs) noexcept
    {
        return lhs.derived().less_than(rhs.derived());
    }
protected:
    friend Derived;
    constexpr IteratorFacade() noexcept = default;
    Derived& derived() noexcept { return *static_cast<Derived*>(this); }
    const Derived& derived() const noexcept { return *static_cast<Derived const*>(this); }

};


int main()
{

}
