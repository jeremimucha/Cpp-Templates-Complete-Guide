#pragma once

#include "expr_array.hpp"
#include "expr_types.hpp"

// addition of two arrays
template<typename T, typename R1, typename R2>
Array<T, A_Add<T,R1,R2>>
operator+(Array<T,R1> const& a, Array<T,R2> const& b)
{
    return Array<T, A_Add<T,R1,R2>>{A_Add<T,R1,R2>(a.rep(), b.rep())};
}

// multilplication of two arrays
template<typename T, typename R1, typename R2>
Array<T, A_Mult<T,R1,R2>>
operator*(Array<T,R1> const& a, Array<T,R2> const& b)
{
    return Array<T, A_Mult<T,R1,R2>>{A_Mult<T,R1,R2>(a.rep(), b.rep())};
}

// multiplication of scalar and Array:
template<typename T, typename R2>
Array<T, A_Mult<T,A_Scalar<T>,R2>>
operator*(T const& s, Array<T,R2> const& b)
{
    return Array<T, A_Mult<T,A_Scalar<T>,R2>>
            { A_Mult<T, A_Scalar<T>, R2>{A_Scalar<T>{s}, b.rep()} };
}

// multiplication of array and scalar
template<typename T, typename R1>
Array<T, A_Mult<T, R1, A_Scalar<T>>>
operator*(Array<T,R1> const& a, T const& s)
{
    return Array<T, A_Mult<T, R1, A_Scalar<T>>>
            { A_Mult<T, R1, A_Scalar<T>>{a.rep(), A_Scalar<T>{s}} };
}

template<typename T, typename R2>
Array<T, A_Add<T, A_Scalar<T>, R2>>
operator+(A_Scalar<T> const& s, Array<T,R2> const& b)
{
    return Array<T, A_Add<T, A_Scalar<T>, R2>>
            { A_Add<T, A_Scalar<T>, R2>{A_Scalar<T>{s}, b.rep()} };
}

// etc...
