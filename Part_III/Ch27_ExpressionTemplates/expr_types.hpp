#pragma once

#include <cstddef>
#include <cassert>
#include "expr_fwd.hpp"

// The goal is to encode the expression 1.2*x + x*y in a template expression
// so that all the operations can be applied in one loop:
// A_Add<A_Mult<A_Scalar<double>, Array<double>>,
//       A_Mult<Array<double>, Array<double>>>

// include helper class traits template to select whether to refer to an
// expression template node either by value or by reference
#include "expr_traits.hpp"

// class for objects that represent the addition of two operands
template<typename T, typename OP1, typename OP2>
class A_Add {
private:
    typename A_Traits<OP1>::ExprRef op1;    // first operand
    typename A_Traits<OP2>::ExprRef op2;    // second operand

public:
    // Construct initializes referencs to operands
    A_Add(OP1 const& a, OP2 const& b)
        : op1{a}, op2{b} { }

    // compute sum when value requested
    T operator[] (std::size_t idx) const {
        return op1[idx] + op2[idx];
    }

    // size is maximum size
    std::size_t size() const {
        assert (op1.size() == 0 || op2.size() == 0
                || op1.size() == op2.size());
        return op1.size() != 0 ? op1.size() : op2.size();
    }
};

// class for objects that represent the multiplication of two operands
template<typename T, typename OP1, typename OP2>
class A_Mult {
private:
    typename A_Traits<OP1>::ExprRef op1;
    typename A_Traits<OP2>::ExprRef op2;

public:
    A_Mult(OP1 const& a, OP2 const& b)
        : op1{a}, op2{b} { }

    // compute product when value requested
    T operator[] (std::size_t idx) const {
        return op1[idx] * op2[idx];
    }

    std::size_t size() const {
        assert (op1.size() == 0 || op2.size() == 0
                || op1.size() == op2.size());
        return op1.size() != 0 ? op1.size() : op2.size();
    }
};

// class for objects that represent multiplication with a scalar
template<typename T>
class A_Scalar {
private:
    T const& s; // value of the scalar

public:
    // ctor
    constexpr A_Scalar(T const& v)
        : s{v} { }

    // for index operations, the scalar is the value of each element
    constexpr T const& operator[] (std::size_t) const {
        return s;
    }

    // scalars have zero as size
    constexpr std::size_t size() const { return 0; }
};


// A type that enables expressions like:
//   x[y] = 2*x[y];
// Which means that the produced result is writable
template<typename T, typename A1, typename A2>
class A_Subscript
{
public:
    // ctor
    A_Subscript(A1 const& a, A2 const& b)
        : a1{a}, a2{b} { }

    // process subscription when value requested
    // decltype(auto) correctly hadnles prvalues / lvalues
    decltype(auto) operator[](std::size_t idx) const
    {
        return a1[a2[idx]];
    }

    T& operator[](std::size_t idx)
    {
        return a1[a2[idx]];
    }

    // size is size of inner array
    std::size_t size() const { return a2.size(); }

private:
    A1 const& a1;       // reference to first operand
    A2 const& a2;       // reference to second operand
};
