#pragma once

#include <utility>
#include "typelist/typelist.hpp"
#include "typelist/typelist_algorithm.hpp"
#include "findindexof.hpp"
#include "variant_fwd.hpp"


template<typename T, typename... Types>
class VariantChoice
{
private:
    using Derived = Variant<Types...>;
    Derived& getDerived() { return *static_cast<Derived*>(this); }
    Derived const& getDerived() const { return *static_cast<Derived const*>(this); }

protected:
    // compute the discriminator to be used for this type
    // use index + 1 - discriminator == 0 is reserved for variant holding no valid type
    static constexpr inline unsigned Discriminator = find_index_of_v<typelist<Types...>, T> + 1;
public:
    constexpr VariantChoice() noexcept { }
    VariantChoice(T const& value);
    VariantChoice(T&& value);
    bool destroy();
    Derived& operator=(T const& value);
    Derived& operator=(T&& value);
};

// construction
/* --------------------------------------------------------------------------------------------- */
template<typename T, typename... Types>
VariantChoice<T, Types...>::VariantChoice(T const& value)
{
    // place value in buffer and set type discriminator:
    new(getDerived().getRawBuffer()) T{value};
    getDerived().setDiscriminator(Discriminator);
}

template<typename T, typename... Types>
VariantChoice<T, Types...>::VariantChoice(T&& value)
{
    // place moved value in buffer and set type discriminator:
    new(getDerived().getRawBuffer()) T{std::move(value)};
    getDerived().setDiscriminator(Discriminator);
}
/* --------------------------------------------------------------------------------------------- */

// destruction
/* --------------------------------------------------------------------------------------------- */
template<typename T, typename... Types>
bool VariantChoice<T, Types...>::destroy()
{
    if (getDerived().getDiscriminator() == Discriminator) {
        // if type matches, call placement delete:
        getDerived().template getBufferAs<T>()->~T();
        return true;
    }
    return false;
}
/* --------------------------------------------------------------------------------------------- */

// assignment
/* --------------------------------------------------------------------------------------------- */
template<typename T, typename... Types>
auto VariantChoice<T,Types...>::operator= (T const& value) -> Derived&
{
    if (getDerived().getDiscriminator() == Discriminator) {
        // assign new value of same type, also handles self-assignment
        *getDerived().template getBufferAs<T>() = value;
    }
    else {
        // assign new value of different type:
        getDerived().destroy();     // try destroy() for all types
        new(getDerived().getRawBuffer()) T{value};  // place new value
        getDerived().setDiscriminator(Discriminator);
    }
    return getDerived();
}

template<typename T, typename... Types>
auto VariantChoice<T,Types...>::operator= (T&& value) -> Derived&
{
    if (getDerived().getDiscriminator() == Discriminator) {
        // assign new value of same type, also handles self assignment
        *getDerived().template getBufferAs<T>() = std::move(value);
    }
    else {
        // assign new value of different type:
        getDerived().destroy();
        new(getDerived().getRawBuffer()) T{std::move(value)};
        getDerived().setDiscriminator(Discriminator);
    }
    return getDerived();
}
/* --------------------------------------------------------------------------------------------- */
