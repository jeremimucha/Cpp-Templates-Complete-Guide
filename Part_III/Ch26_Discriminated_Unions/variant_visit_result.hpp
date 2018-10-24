#pragma once

#include "variant_fwd.hpp"
#include "typelist/typelist.hpp"
#include "typelist/accumulate.hpp"
#include "commontype.hpp"
#include <type_traits>


// an explicitly-provided visitor result type:
template<typename R, typename Visitor, typename... ElementTypes>
struct VisitResultT
{
    using type = R;
};

template<typename R, typename Visitor, typename... ElementTypes>
using VisitResult = typename VisitResultT<R, Visitor, ElementTypes...>::type;


// the result type produced when calling a visitor with a value of type T:
template<typename Visitor, typename T>
using VisitElementResult = decltype(std::declval<Visitor>()(std::declval<T>()));

// the common result type for a visitor called with each of the given element types:
template<typename Visitor, typename... ElementTypes>
struct ComputedResultType
{
private:
    using ResultTypes = typelist<VisitElementResult<Visitor,ElementTypes>...>;
public:
    using type = accumulate_t<pop_front_t<ResultTypes>, common_type, front_t<ResultTypes>>;
};

template<typename Visitor, typename... ElementTypes>
struct VisitResultT<typename ComputedResultType<Visitor,ElementTypes...>::type,
                    Visitor,
                    ElementTypes...>
{
    using type = typename ComputedResultType<Visitor, ElementTypes...>::type;
};


namespace visit_result_std
{
template<typename R, typename Visitor, typename... ElementTypes>
struct stdVisitResultT
{
    using type = R;
};

template<typename R, typename Visitor, typename... ElementTypes>
using stdVisitResult = typename VisitResultT<R, Visitor, ElementTypes...>::type;

// or using the standard library
template<typename Visitor, typename... ElementTypes>
struct stdVisitResultT<std::common_type_t<VisitElementResult<Visitor,ElementTypes>...>,
                  Visitor,
                  ElementTypes...>
{
    using type = std::common_type_t<VisitElementResult<Visitor,ElementTypes>...>;
};

} // visit_result_std
