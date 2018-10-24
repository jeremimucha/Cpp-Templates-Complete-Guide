#pragma once

#include <exception>    // for get()
#include "variant_fwd.hpp"
#include "variantstorage.hpp"
#include "variantchoice.hpp"
#include "variant_visit_result.hpp"
#include "typelist/typelist.hpp"


// Variant declaration / definition.
// Note the double expansion of parameter packs in inherited VariantChoice.
// In effect Variant will inherit from
// VariantChoice<T1, Types...>, VariantChoice<T2, Types...>, VariantChoice<TN, Types...>
template<typename... Types>
class Variant
    : private VariantStorage<Types...>,
      private VariantChoice<Types, Types...>...
{
private:
    template<typename T, typename... OtherTypes>
    friend class VariantChoice; // enable CRTP

public:
    template<typename T> bool is() const;
    template<typename T> T& get() &;
    template<typename T> T const& get() const&;
    template<typename T> T&& get() &&;

    template<typename R = ComputedResultType, typename Visitor>
    VisitResult<R, Visitor, Types&...> visit(Visitor&& vis) &;
    template<typename R = ComputedResultType, typename Visitor>
    VisitResult<R, Visitor, Types const&...> visit(Visitor&& vis) const&;
    template<typename R = ComputedResultType, typename Visitor>
    VisitResult<R, Visitor, Types&&...> visit(Visitor&& vis) &&;

    // inherit all of the inherited from constructors, to allow implicit conversion to
    // the held type
    using VariantChoice<Types, Types...>::VariantChoice...;

    Variant();
    Variant(Variant const& source);
    Variant(Variant&& source);

    template<typename... SourceTypes>
    Variant(Variant<SourceTypes...> const& source);

    template<typename... SourceTypes>
    Variant(Variant<SourceTypes...>&& source);

    // inherit all assignment operators
    using VariantChoice<Types,Types...>::operator=...;
    Variant& operator=(Variant const& source);
    Variant& operator=(Variant&& source);
    template<typename... SourceTypes>
    Variant& operator=(Variant<SourceTypes...> const& source);
    template<typename... SourceTypes>
    Variant& operator=(Variant<SourceTypes...>&& source);

    bool empty() const;

    ~Variant() { destroy(); }

    void destroy();
};


// test
/* --------------------------------------------------------------------------------------------- */
template<typename... Types>
  template<typename T>
bool Variant<Types...>::is() const
{
    return this->getDiscriminator() == VariantChoice<T,Types...>::Discriminator;
}

template<typename... Types>
bool Variant<Types...>::empty() const
{
    return this->getDiscriminator() == 0;
}

/* --------------------------------------------------------------------------------------------- */

// destuction
/* --------------------------------------------------------------------------------------------- */
template<typename... Types>
void Variant<Types...>::destroy()
{
    // call destroy() on each VariantChoice base class; at most one will succeed:
    (... , VariantChoice<Types, Types...>::destroy());
    // indicate that the variant does not store a value
    this->setDiscriminator(0);
}
/* --------------------------------------------------------------------------------------------- */


// access
/* --------------------------------------------------------------------------------------------- */
class EmptyVariant : public std::exception { };

template<typename... Types>
  template<typename T>
T& Variant<Types...>::get() &
{
    if (empty()) {
        throw EmptyVariant();
    }

    assert(is<T>());
    return *this->template getBufferAs<T>();
}

template<typename... Types>
  template<typename T>
T const& Variant<Types...>::get() const&
{
    if (empty()) {
        throw EmptyVariant();
    }

    assert(is<T>());
    return *this->template getBufferAs<T>();
}

template<typename... Types>
  template<typename T>
T&& Variant<Types...>::get() &&
{
    if (empty()) {
        throw EmptyVariant();
    }

    assert(is<T>());
    return std::move(*this->template getBufferAs<T>());
}
/* --------------------------------------------------------------------------------------------- */

// visit
/* --------------------------------------------------------------------------------------------- */
template<typename R, typename V, typename Visitor,
         typename Head, typename... Tail>
R variantVisitImpl(V&& variant, Visitor&& vis, typelist<Head,Tail...>)
{
    if (variant.template is<Head>()) {
        return static_cast<R>(
                std::forward<Visitor>(vis)(
                    std::forward<V>(variant).template get<Head>()));
    }
    else if constexpr (sizeof...(Tail) > 0) {
        return variantVisitImpl<R>(std::forward<V>(variant)
                                  std::forward<Visitor>(vis),
                                  typelist<Tail...>{});
    }
    else {
        throw EmptyVariant{};
    }
}

template<typename... Types>
  template<typename R, typename Visitor>
VisitResult<R, Visitor, Types&...>
Variant<Types...>::visit(Visitor&& vis) &
{
    using Result = VisitResult<R, Visitor, Types&...>;
    return variantVisitImpl<Result>(*this, std::forward<Visitor>(vis), typelist<Types...>{});
}

template<typename... Types>
  template<typename R, typename Visitor>
VisitResult<R, Visitor, Types const&...>
Variant<Types...>::visit(Visitor&& vis) const&
{
    using Result = VisitResult<R, Visitor, Types const&...>;
    return variantVisitImpl<Result>(*this, std::forward<Visitor>(vis), typelist<Types...>{});
}

template<typename... Types>
  template<typename R, typename Visitor>
VisitResult<R, Visitor, Types&&...>
Variant<Types...>::visit(Visitor&& vis) &&
{
    using Result = VisitResult<R, Visitor, Types&&...>;
    return variantVisitImpl<Result>(std::move(*this),
                                    std::forward<Visitor>(vis),
                                    typelist<Types...>{});
}
/* --------------------------------------------------------------------------------------------- */

// Construction
/* --------------------------------------------------------------------------------------------- */
template<typename... Types>
Variant<Types...>::Variant()
{
    *this = front_t<typelist<Types...>>{};
}

template<typename... Types>
Variant<Types...>::Variant(Variant const& source)
{
    if (!source.empty()) {
        source.visit([&](auto const& value)) {
            *this = value;
        }
    }
}

template<typename... Types>
Variant<Types...>::Variant(Variant&& source)
{
    if (!source.empty()) {
        source.visit([&](auto&& value)) {
            *this = std::move(value);
        }
    }
}

template<typename... Types>
  template<typename... SourceTypes>
Variant<Types...>::Variant(Variant<SourceTypes...> const& source)
{
    if !(source.empty()) {
        source.visit([&](auto const& value) {
            *this = value;
        });
    }
}

template<typename... Types>
Variant<Types...>& Variant<Types...>::operator= (Variant const& source)
{
    if (!source.empty()) {
        source.visit([&](auto const& value){
            *this = value;
        })
    }
    else {
        destroy();
    }
    return *this;
}

template<typename... Types>
Variant<Types...>& Variant<Types...>::operator= (Variant&& source)
{
    if (!source.empty()) {
        source.visit([&](auto&& value){
            *this = std::move(value);
        })
    }
    else {
        destroy();
    }
    return *this;
}
/* --------------------------------------------------------------------------------------------- */